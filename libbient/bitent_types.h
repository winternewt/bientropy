#pragma once
#ifndef BITENT_TYPES_H
#define BITENT_TYPES_H

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

#include <cstdint>
#include <cstdlib>
#include <climits>
#include <map>

static const uint_fast16_t CACHE_MAX_LENGTH = 20; //configure memory consumption
typedef double FloatT; //precision choice
typedef uint32_t Bits; //define bitstring

typedef struct bientropy {
    FloatT bien; // BiEntropy
    FloatT tbien; // TBiEntropy
    FloatT t; //auxilary field, bitstring length
} bientropy;

template<typename T>
class Key
{
public:
    Key(T bits, uint_fast8_t stop, uint_fast8_t start)
    {
        this->bits = bits;
        this->stop = stop;
        this->start = start;
    }
    T bits;
    uint16_t stop;
    uint16_t start;
    bool operator<(const Key& k) const
    {
        if (this->bits == k.bits)
        {
            if (this->stop == k.stop)
            {
                return this->start < k.start;
            }
            return this->stop < k.stop;
        }
        return this->bits < k.bits;
    }
};

template <typename T>
using mapCache = std::map<Key<T>, bientropy>; // cache of precalculated bientopies
template <typename T>
using mapCacheIterator = typename mapCache<T>::const_iterator;

#endif
