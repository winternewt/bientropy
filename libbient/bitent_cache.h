#pragma once
#ifndef BITENT_CACHE_H
#define BITENT_CACHE_H
/*
 * Copyright (c) 2021 Newton Winter
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <vector>
#include <bitent_procs.h>


template <typename T>
class ThinCacheNode {
public:
	FloatT eg; //product
	T idx; //refernce to next derivative
};

template <typename T>
using ThinCacheLayer = std::vector<ThinCacheNode<T>>;


template <typename T>
class ThinCache {
private:
	const uint_fast8_t top;
	std::vector<ThinCacheLayer<T>> layers; //index is bin-derivative

	constexpr ThinCacheNode<T> getNode(const T input, const uint_fast8_t length) const {
		T idx = (input % 2 == 1) ? trim_bitsT<T>(~input, length) : input;
		return layers[length][idx / 2];
	}

	template <CALC_SELECTOR S>
	inline bientropy __get_bientropy(const T input, const uint_fast8_t length, const uint_fast8_t offset) const {
		bientropy cache = {};
		uint_fast8_t cl = (length > top) ? top + offset : length + offset;
		T idx = input;
		for (size_t k = offset; k <= cl - BIENTROPY_MINLENGTH; ++k)
		{
			const ThinCacheNode<T> node = getNode(idx, cl - k);
			one_or_both<S>(cache, node.eg, uint_fast8_t(k));
			idx = node.idx;
		}
		cache.t = std::copysign(idx % 2, (idx % 2) * 2.0 - 1.0); //-1 for 0 and +1 for 1
		return cache;
	}
public:
	ThinCache(const uint_fast8_t length) :
		top(length > CACHE_MAX_LENGTH ? CACHE_MAX_LENGTH : ((length < BIENTROPY_MINLENGTH) ? BIENTROPY_MINLENGTH : length)),
		layers(top + 1) // last layer top - bottom is len==2
	{
		size_t i, k, j;
		for (i = 1; i < layers.size(); i++)
		{
			k = 1ULL << i;
			layers[i].resize(k / 2); //only even variants
			for (j = 0; j < k; j += 2)
			{
				layers[i][j / 2].eg = egLUT[((i - 1) << stc) | popT<T>(j)];
				layers[i][j / 2].idx = bin_derivT<T>((T)j, i);
			}
		}

	}
	uint_fast8_t size() const { return top; };

	inline bientropy get_bientropy(const T input, const uint_fast8_t length, const uint_fast8_t offset) const {
		return __get_bientropy<BOTH>(input, length, offset);
	}
	inline FloatT get_bien(const T input, const uint_fast8_t length, const uint_fast8_t offset) const {
		return __get_bientropy<BOTH>(input, length, offset);
	}
	inline FloatT get_bienf(const T input, const uint_fast8_t length, const uint_fast8_t offset) const {
		bientropy cache = __get_bientropy<BIEN>(input, length, offset);
		return std::copysign(cache.bien, cache.t);
	}
	inline bientropy get_tbien(const T input, const uint_fast8_t length, const uint_fast8_t offset) const {
		return __get_bientropy<TBIEN>(input, length, offset);
	}
	inline FloatT get_tbienf(const T input, const uint_fast8_t length, const uint_fast8_t offset) const {
		bientropy cache = __get_bientropy<TBIEN>(input, length, offset);
		return std::copysign(cache.bien, cache.t); //-1 for 0 and +1 for 1 if lat bit
	}
};


inline bientropy catBTBien(const bientropy& product, const bientropy& cache, const uint_fast8_t length, const uint_fast8_t stop) { // last deriv stored in t sign
	bientropy res = {};
	res.bien = catBienS(product.bien, cache.bien, length, stop);
	res.tbien = catTBienS(product.tbien, cache.tbien, length, stop);
	res.t = std::copysign((FloatT)length, cache.t * product.t);
	return res;
}

template <typename T>
constexpr uint_fast8_t getStop(const ThinCache<T>* cache, const uint_fast8_t length) {
	return (cache == nullptr) ? 1 :
		(length > cache->size() ?
			cache->size() : length);
}

template <typename T>
constexpr bientropy getBientropyCached(const ThinCache<T>* cache, T bits, const uint_fast8_t length, const uint_fast8_t stop) {
	if (cache == nullptr || stop == 1)
		return {};
	uint_fast8_t offset = length - stop;
	return cache->get_bientropy(bits, length, offset);
}

template <typename T>
constexpr bientropy getBientropyT(const ThinCache<T>* cache, T bits, const uint_fast8_t length) {
	uint_fast8_t stop = getStop(cache, length);
	bientropy product = {};
	T deriv = getBientropyToStop<T, BOTH>(product, bits, length, stop);
	bientropy cache_product = getBientropyCached(cache, deriv, length, stop);
	return catBTBien(product, cache_product, length, 1);
}


#endif
