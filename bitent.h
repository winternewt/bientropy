#ifndef BITOPS_H
#define BITOPS_H
#pragma once

#include <math.h>
#include <bittwid.h> 
#include <limits.h>     /* CHAR_BIT */

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


#define BIT_MASK(__TYPE__, __ONE_COUNT__) ((__TYPE__) (-((__ONE_COUNT__) != 0))) & (((__TYPE__) -1) >> ((sizeof(__TYPE__) * CHAR_BIT) - (__ONE_COUNT__)))

// stackoverflow.com/questions/1392059/algorithm-to-generate-bit-mask
/*
static const uint64_t masks64[64] = { //precomputed
	0x0000000000000001UL, 	0x0000000000000003UL,	0x0000000000000007UL,	0x000000000000000fUL,	0x000000000000001fUL,	0x000000000000003fUL,	0x000000000000007fUL,	0x00000000000000ffUL,
	0x00000000000001ffUL,	0x00000000000003ffUL,	0x00000000000007ffUL,	0x0000000000000fffUL,	0x0000000000001fffUL,	0x0000000000003fffUL,	0x0000000000007fffUL,	0x000000000000ffffUL,
	0x000000000001ffffUL,	0x000000000003ffffUL,	0x000000000007ffffUL,	0x00000000000fffffUL,   0x00000000001fffffUL,	0x00000000003fffffUL,	0x00000000007fffffUL,	0x0000000000ffffffUL,
	0x0000000001ffffffUL,	0x0000000003ffffffUL,	0x0000000007ffffffUL,	0x000000000fffffffUL,   0x000000001fffffffUL,	0x000000003fffffffUL,	0x000000007fffffffUL,	0x00000000ffffffffUL,
	0x00000001ffffffffUL,	0x00000003ffffffffUL,	0x00000007ffffffffUL,	0x0000000fffffffffUL,   0x0000001fffffffffUL,	0x0000003fffffffffUL,	0x0000007fffffffffUL,	0x000000ffffffffffUL,
	0x000001ffffffffffUL,	0x000003ffffffffffUL,	0x000007ffffffffffUL,	0x00000fffffffffffUL,   0x00001fffffffffffUL,	0x00003fffffffffffUL,	0x00007fffffffffffUL,	0x0000ffffffffffffUL,
	0x0001ffffffffffffUL,	0x0003ffffffffffffUL,	0x0007ffffffffffffUL,	0x000fffffffffffffUL,   0x001fffffffffffffUL,	0x003fffffffffffffUL,	0x007fffffffffffffUL,	0x00ffffffffffffffUL,
	0x01ffffffffffffffUL,	0x03ffffffffffffffUL,	0x07ffffffffffffffUL,	0x0fffffffffffffffUL,   0x1fffffffffffffffUL,	0x3fffffffffffffffUL,	0x7fffffffffffffffUL,	0xffffffffffffffffUL
};*/

static const uint_fast64_t lsb_set32 = 0x80000000U;
static const uint_fast64_t lsb_set64 = 0x8000000000000000UL;

struct bientropy {
	double bien;
	double tbien;
	double t;
};
template <typename T>
using f_cachecallback = bientropy (*)(const T, const uint_fast8_t, void*); //callback to retrieve from cache

template <typename T>
static bientropy __dummy(const T bits, const uint_fast8_t length, void* ctx){
	bientropy dummy = { NAN };
	if (length == 1)
		dummy = { 0. };
	return dummy;
};

template <typename T> 
inline T trim_bitsT(const T bits, const uint_fast8_t length) {
	T mask = (length >= sizeof(T) * CHAR_BIT) ? BIT_MASK(T, sizeof(T) * CHAR_BIT) : BIT_MASK(T, length);
	return bits & mask;
}

inline uint32_t trim_bits(const uint32_t bits, const uint_fast8_t length) {
	return trim_bitsT<uint32_t>(bits, length);
}

inline uint64_t trim_bits(const uint64_t bits, const uint_fast8_t length) {
	return trim_bitsT<uint64_t>(bits, length);
}

template <typename T>
inline T bin_derivT(const T bits, const uint_fast8_t length) { // https://sci-hub.do/https://journals.sagepub.com/doi/abs/10.1177/003754978905300307
	if (length < 2)
		return trim_bitsT<T>(bits, length);
	return trim_bitsT<T>((bits ^ (bits << 1)), length) >> 1;  // bits xor (bits shl 1) then remove excessive msb and lsb  
}

inline uint32_t bin_deriv(const uint32_t bits, const uint_fast8_t length) {
	return bin_derivT<uint32_t>(bits, length);
}

inline uint64_t bin_deriv(const uint64_t bits, const uint_fast8_t length) {
	return bin_derivT<uint64_t>(bits, length);
}

template <typename T>
inline T bin_deriv_kT(const T bits, const uint_fast8_t length, const uint_fast8_t k) {
	return (k == 0 ? trim_bitsT<T>(bits, length) : bin_derivT<T>(bin_deriv_kT<T>(bits, length, k - 1), length)); // the kth binary derivative of length n-k where n is the length of the input
}

inline uint_fast32_t bin_deriv_k(const uint_fast32_t bits, const uint_fast8_t length, const uint_fast8_t k) {
	return bin_deriv_kT<uint_fast32_t>(bits, length, k);
}

inline uint_fast64_t bin_deriv_k(const uint_fast64_t bits, const uint_fast8_t length, const uint_fast8_t k) {
	return bin_deriv_kT<uint_fast64_t>(bits, length, k);
}

template <typename T>
T bienTtoStepT(const T bits, bientropy* cache, const uint_fast8_t length, const uint_fast8_t stop, const uint_fast8_t offset) {
	if (length < 2 || stop < 1 || stop >= length || cache == NULL)
		return bits;
	T o, res = trim_bitsT<T>(bits, length);
	double p, e, g, l;
	uint_fast8_t n = length;
	cache->t = 0.;
	cache->bien = 0;
	cache->tbien = 0;
	for (uint_fast8_t k = 0; k < length - stop; ++k, --n) {
		o = popT<T>(res); // count of 0;
		p = (double)o / n;
		e = (o == 0) ? 0 : -p * log2(p);
		g = (o == n) ? 0 : -1.0 * (1UL - p) * log2(1UL - p);
		l = log2(2ULL + k + offset);
		cache->t += l;
		cache->bien += (e + g) * (1ULL << k);
		cache->tbien += (e + g) * l;
		res = bin_derivT<T>(res, n);
	}
	return res;
}

uint64_t bienTtoStep(const uint64_t bits, bientropy* cache, const uint_fast8_t length, const uint_fast8_t stop, const uint_fast8_t offset) {
	return bienTtoStepT<uint64_t>(bits, cache, length, stop, offset);
}

uint32_t bienTtoStep(const uint32_t bits, bientropy* cache, const uint_fast8_t length, const uint_fast8_t stop, const uint_fast8_t offset) {
	return bienTtoStepT<uint32_t>(bits, cache, length, stop, offset);
}

bientropy bienTcache(const bientropy* cache, const bientropy* precalc, const uint_fast8_t length, const uint_fast8_t stop) {
	bientropy res = { NAN };
	if (precalc == NULL || cache == NULL)
		return res;
	/*
	res.t = cache->t; //cahced log sum
	for (uint_fast8_t k = length - stop; k < length - 1; ++k) {
		res.t += log2(2ULL + k); //continue summation
	}*/
	res.bien = (cache->bien + precalc->bien * (1ULL << (length-stop))) / ((1ULL << (length - 1ULL)) - 1ULL);	
	res.tbien = (cache->tbien + precalc->tbien) / (cache->t + precalc->t);
	return res;
}

template <typename T>
bientropy bienTcached(const T bits, const uint_fast8_t length, const uint_fast8_t stop, f_cachecallback<T> callback, void * ctx) {
	bientropy cache = { NAN };
	if (length < 2 || stop < 1 || stop >= length || callback == NULL)
		return cache;
	
	bientropy precalc = callback( //get from cache
		bienTtoStepT<T>(bits, &cache, length, stop, 0), //calculate before stop, return binary derivative
		stop,
		ctx
	);
	return bienTcache(&cache, &precalc, length, stop);
}

template <typename T>
bientropy bienT(const T bits, const uint_fast8_t length) {
	return bienTcached<T>(bits, length, 1, &__dummy<T>, NULL);
}

bientropy bien(const uint64_t bits, const uint_fast8_t length) {
	return bienT<uint64_t>(bits,length);
}

bientropy bien(const uint32_t bits, const uint_fast8_t length) {
	return bienT<uint32_t>(bits, length);
}

bientropy bien(const uint16_t bits, const uint_fast8_t length) {
	return bienT<uint16_t>(bits, length);
}

#endif