#pragma once
#ifndef BITENT_PROCS2_H
#define BITENT_PROCS2_H

#include <bitent_lut.h>


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

enum CALC_SELECTOR {
	BOTH,
	BIEN,
	TBIEN,
	PRECISION_BOTH
};

template <typename T>
inline T bin_derivT(const T bits, const uint_fast16_t length) { //https://journals.sagepub.com/doi/abs/10.1177/003754978905300307
	if (length < 2)
		return trim_bitsT<T>(bits, length);
	return trim_bitsT<T>((bits ^ (bits << 1)) >> 1, length - 1);  // bits xor (bits shl 1) then remove excessive msb and lsb  
}

template <typename T>
inline T bin_deriv_kT(const T bits, const uint_fast16_t length, const uint_fast16_t k) {
	return (k == 0 ? trim_bitsT<T>(bits, length) : bin_derivT<T>(bin_deriv_kT<T>(bits, length, k - 1), length - k)); // the kth binary derivative of length n-k where n is the length of the input
}

constexpr FloatT catBienS(const FloatT& product, const FloatT& cache, const uint_fast8_t length, const uint_fast8_t stop) { // last deriv stored in sign
	return (product + cache) / ((pow2LUT[length] - pow2LUT[stop]) - 1ULL);
}

constexpr FloatT catTBienS(const FloatT& product, const FloatT& cache, const uint_fast8_t length, const uint_fast8_t stop) { // last deriv stored in sign
	return (product + cache) / (logSumLUT[length] - logSumLUT[stop]);
}


template <CALC_SELECTOR S>
constexpr void one_or_both(bientropy& cache, const FloatT eg, const uint_fast8_t k) {
	switch (S) {
	case BOTH:
		cache.tbien += eg * log2LUT[2U + k];
		[[fallthrough]];
	case BIEN:
		cache.bien += eg * pow2LUT[k];
		break;
	case TBIEN:
		cache.tbien += eg * log2LUT[2U + k];
		break;
	case PRECISION_BOTH:
		cache.tbien = fma(eg, log2LUT[2U + k], cache.tbien);
		cache.bien = fma(eg, pow2LUT[k], cache.bien);
		[[fallthrough]];
	default:
		break;
	};
}


template <typename T, CALC_SELECTOR S>
constexpr T getBientropyToStop(bientropy& product, const T bits, const uint_fast8_t length, const uint_fast8_t stop) {
	if (length < 2 || stop < 1 || stop >= length )
		return trim_bitsT<T>(bits, length);
	T res = trim_bitsT<T>(bits, length);
	FloatT eg;
	for (uint_fast8_t k = 0, n = length; k < length - stop; ++k, --n) {
		eg = egLUT[((n - 1) << stc) | popT<T>(res)]; //encode 2D index
		one_or_both<S>(product, eg, k);
		res = bin_derivT<T>(res, n);
	}
	if (stop == 1)
		product.t = (res % 2) * length - (FloatT)length;
	return res;
}

template <typename T, CALC_SELECTOR S>
constexpr T getBientropyToEnd(bientropy& product, const T bits, const uint_fast8_t length) {
	if (length < 2)
		return trim_bitsT<T>(bits, length);
	T res = trim_bitsT<T>(bits, length);
	FloatT eg;
	for (uint_fast8_t k = 0, n = length; k < length - 1; ++k, --n) {
		eg = egLUT[((n - 1) << stc) | popT<T>(res)]; //encode 2D index
		one_or_both<S>(product, eg, k);
		res = bin_derivT<T>(res, n);
	}
	product.bien /= pow2LUT[length] - (pow2LUT[1] + 1U);
	product.tbien /= logSumLUT[length];
	product.t = int_fast32_t((res % 2) * length) - length;
	return res;
}

template <typename T>
constexpr bientropy getBientropyPreciseT(T bits, const uint_fast8_t length) {
	bientropy product = {};
	getBientropyToEnd<T, PRECISION_BOTH>(product, bits, length);
	return product;
}


template <typename T>
constexpr bientropy getBientropyT(T bits, const uint_fast8_t length) {
	bientropy product = {};
	getBientropyToEnd<T,BOTH>(product, bits, length);
	return product;
}

template <typename T>
constexpr bientropy getOnlyBientropyT(T bits, const uint_fast8_t length) {
	bientropy product = {};
	getBientropyToEnd<T,BIEN>(product, bits, length);
	return product;
}

template <typename T>
constexpr bientropy getOnlyTBientropyT(T bits, const uint_fast8_t length) {
	bientropy product = {};
	getBientropyToEnd<T, TBIEN>(product, bits, length);
	return product;
}


constexpr FloatT tbienMeanWeight(const FloatT val, const uint_fast8_t length) {  //tbien divided between [0.5; 1] so that tbienWeight median is ~1 on any lentgth
	if (length < BIENTROPY_MINLENGTH || length > BITENT_STAT_LENGTH)
		return NAN;
	FloatT res = val / (tbient_mean[length]); 
	return (2.0L - res); // chaos [2-1/mean ;1.0) U order (1.0; 2.0] 
}


constexpr FloatT tbienMedianWeight(const FloatT val, const uint_fast8_t length) {  //tbien divided between [0.5; 1] so that tbienWeight median is ~1 on any lentgth
	if (length < BIENTROPY_MINLENGTH || length > BITENT_STAT_LENGTH)
		return NAN;
	FloatT res = val / (tbient_median[length]); 
	return (2.0L - res); // chaos [1/median ;1.0) U order (1.0; 2.0] 
}

template <typename T>
constexpr FloatT getTbienMeanWeightT(T bits, const uint_fast8_t length) {
	bientropy product = {};
	getBientropyToEnd<T, TBIEN>(product, bits, length);
	return std::copysign(tbienMeanWeight(product.tbien, length), product.t);
}

template <typename T>
constexpr FloatT getTbienMedianWeightT(T bits, const uint_fast8_t length) {
	bientropy product = {};
	getBientropyToEnd<T, TBIEN>(product, bits, length);
	return std::copysign(tbienMedianWeight(product.tbien, length), product.t);
}


constexpr FloatT tbienMeanWeight(const bientropy val, const uint_fast8_t length) {  //tbien divided between [0.5; 1] so that tbienWeight median is ~1 on any lentgth
	return tbienMeanWeight(val.tbien, length);
}

constexpr FloatT tbienMedianWeight(const bientropy val, const uint_fast8_t length) {  //tbien divided between [0.5; 1] so that tbienWeight median is ~1 on any lentgth
	return tbienMedianWeight(val.tbien, length);
}



#endif
