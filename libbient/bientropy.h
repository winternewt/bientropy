#pragma once
#ifndef BIENTROPY_H
#define BIENTROPY_H

#include <bitent_types.h>
#include <bitent_procs.h>

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

typedef mapCache<Bits> bitsCache;
typedef mapCacheIterator<Bits> bitsIterator;

/*! @fn         bitsCache cacheInit(const uint_fast16_t min, const uint_fast16_t max)
 *  @brief      Populates a cache to speed up Bitentropy calculations for a range of input bitsring lengths.
 *  @param      min 8-bit value, minimum lentght of bitstring that will be sent for processing
 *  @param      max 8-bit value, minimum lentght of bitstring that will be sent for processing
 *  @returns    An instance of cache based on Bits type
 */
bitsCache cacheInit(const uint_fast16_t min, const uint_fast16_t max) {
    return cacheInitT<Bits>(min, max);
}

/*! @fn         Bits binary_derivative(const Bits bits, const uint_fast16_t length)
 *  @brief      Returns the next Binary Derivative for bits string, see https://journals.sagepub.com/doi/abs/10.1177/003754978905300307
 *  @param      bits - input Bits string
 *  @param      length - 8-bit value, defines number of bits starting from less significant
 *  @returns    next binary derivative
 */
Bits binary_derivative(const Bits bits, const uint_fast16_t length) { // see https://journals.sagepub.com/doi/abs/10.1177/003754978905300307
    return bin_derivT<Bits>(bits, length);
}

/*! @fn         Bits binary_derivative_kth(const Bits bits, const uint_fast16_t l, const uint_fast16_t k) 
 *  @brief      Returns k-th Binary Derivative for bits string, see https://journals.sagepub.com/doi/abs/10.1177/003754978905300307
 *  @param      bits - input Bits string
 *  @param      length - 8-bit value, defines number of bits starting from less significant
 *  @param      k - number of derivative to stop at. k=0 returns input 
 *  @returns    k-th binary derivative
 */
Bits binary_derivative_kth(const Bits bits, const uint_fast16_t l, const uint_fast16_t k) { 
    return bin_deriv_kT<Bits>(bits, l, k);
}

/*! @fn         calculate_bientropy(const bitsCache* cache, Bits bits, const uint_fast16_t l)v
 *  @brief      Calculates BiEntropy and TBiEntropy values for bits string, using lookup table for speedup if available see https://arxiv.org/abs/1305.0954
 *  @param      cache - set to a value obtined from cacheInit() to speedup. Set to NULL to perform complete computation
 *  @param      bits - input Bits string
 *  @param      l - 8-bit value, defines number of bits starting from less significant
 *  @returns    a struct conating BiEntropy and TBiEntropy values
 */
bientropy calculate_bientropy(const bitsCache* cache, Bits bits, const uint_fast16_t l) { //returns bientropy+tbientropy for bits string with length (l), set cache t
    return getBientropyT<Bits>(cache, bits, l);
}

/*! @fn         get_bientropy_kth(Bits bits, const uint_fast16_t l, const uint_fast16_t k)
 *  @brief      Calculates BiEntropy and TBiEntropy values for bits string, using lookup table for speedup if available see https://arxiv.org/abs/1305.0954
 *  @param      bits - input Bits string
 *  @param      l - 8-bit value, defines number of bits starting from less significant
 *  @param      k - number of the derivative to stop at. k=0 or k>=l returns input same as get_bientropy() 
 *  @returns    a struct conating BiEntropy and TBiEntropy values
 */
bientropy get_bientropy_kth(Bits bits, const uint_fast16_t l, const uint_fast16_t k) { 
    return bienTs<Bits>(bits, l, ((k < l) && (k>0)) ? l-k : 1);
}

/*! @fn         bientropy get_bientropy_single_step(Bits bits, const uint_fast16_t l) 
 *  @brief      Calculates BiEntropy and TBiEntropy values for bits string, using lookup table for speedup if available see https://arxiv.org/abs/1305.0954
 *  @param      bits - input Bits string
 *  @param      l - 8-bit value, defines number of bits starting from less significant
 *  @returns    a struct conating BiEntropy and TBiEntropy values for the first derivative
 */
bientropy get_bientropy_single_step(Bits bits, const uint_fast16_t l) { //returns bientropy+tbientropy for this derivative
    return bienTs<Bits>(bits, l, l - 1);
}

/*! @fn         FloatT get_bientropy(const bitsCache* cache, Bits bits, const uint_fast16_t l)
 *  @brief      Calculates BiEntropy values for bits string, using lookup table for speedup if available see https://arxiv.org/abs/1305.0954
 *  @param      cache - set to a value obtined from cacheInit() to speedup. Set to NULL to perform complete computation
 *  @param      bits - input Bits string
 *  @param      l - 8-bit value, defines number of bits starting from less significant
 *  @returns    a FloatT conating BiEntropy value
 */
FloatT get_bientropy(const bitsCache* cache, Bits bits, const uint_fast16_t l) { //same but only bientropy
    return getBientropyT<Bits>(cache, bits, l).bien;
}

/*! @fn         FloatT get_tbientropy(const bitsCache* cache, Bits bits, const uint_fast16_t l)
 *  @brief      Calculates TBiEntropy values for bits string, using lookup table for speedup if available see https://arxiv.org/abs/1305.0954
 *  @param      bits - input Bits string
 *  @param      cache - set to a value obtined from cacheInit() to speedup. Set to NULL to perform complete computation
 *  @param      l - 8-bit value, defines number of bits starting from less significant
 *  @returns    a FloatT conating TBiEntropy value
 */
FloatT get_tbientropy(const bitsCache* cache, Bits bits, const uint_fast16_t l) { //same but only tbientropy
    return getBientropyT<Bits>(cache, bits, l).tbien;
}

/*! @fn         FloatT get_weight_mean(const bitsCache* cache, Bits bits, const uint_fast16_t l) 
 *  @brief      TBiEntropy based score function, output bounds are: <- chaos [0.0 ;1.0) U (1.0; 2.0] order ->, median value is 1.0 (half of return values are above 1.0, half below)
 *  @param      bits - input Bits string
 *  @param      cache - set to a value obtined from cacheInit() to speedup. Set to NULL to perform complete computation
 *  @param      l - 8-bit value, defines number of bits starting from less significant
 *  @returns    a FloatT conating score value, aligned so that its median value is ~1 on any lentgth
 */
FloatT get_weight_mean(const bitsCache* cache, Bits bits, const uint_fast16_t l) {  
    return  tbienMeanWeight(getBientropyT<Bits>(cache, bits, l) ,l);
}

/*! @fn         FloatT get_tbientropy(const bitsCache* cache, Bits bits, const uint_fast16_t l)
 *  @brief      TBiEntropy based score function,  output bounds are: <- chaos [0.0 ;1.0) U (1.0; 2.0] order ->, mean average of all return values equals 1.0 
 *  @param      bits - input Bits string
 *  @param      cache - set to a value obtined from cacheInit() to speedup. Set to NULL to perform complete computation
 *  @param      l - 8-bit value, defines number of bits starting from less significant
 *  @returns    a FloatT conating score value, aligned so that its mean value is ~1 on any lentgth
 */
FloatT get_weight_median(const bitsCache* cache, Bits bits, const uint_fast16_t l) { 
    return  tbienMedianWeight(getBientropyT<Bits>(cache, bits, l), l); 
}





#endif