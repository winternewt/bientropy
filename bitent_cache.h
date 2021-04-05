#ifndef BITENT_CACHE_H
#define BITENT_CACHE_H
#pragma once
#include <map>
#include <bitent_types.h>
#include <bitent.h>

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
    uint_fast8_t stop;
    uint_fast8_t start;
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

inline uint_fast8_t cacheSize(const uint_fast8_t l) {
    return (l < CACHE_MAX_LENGTH) ? l : CACHE_MAX_LENGTH;
}

template <typename T> 
inline  mapCacheIterator<T> cacheFind(const mapCache<T>* cacheMap, const T bits, const uint_fast8_t start, const uint_fast8_t stop) {
    mapCacheIterator<T> index = cacheMap->find(Key<T>(bits, stop, start));
    if (index == cacheMap->end()) {
        index = cacheMap->find(Key<T>(trim_bitsT<T>(~bits, stop), stop, start)); //mirror has same entropy
    }
    return index;
}

template <typename T>
static bientropy cache_callback(const T bits, const uint_fast8_t start, const uint_fast8_t stop, void* ctx) {
    mapCache<T>* cacheMap = (mapCache<T> *) ctx;
    return cacheFind<T>(cacheMap, bits, start, stop)->second;
};

template <typename T>
inline bool cacheContains(const mapCache<T>* cacheMap, const uint_fast8_t l) {
    return (cacheFind<T>(cacheMap, 0, l, cacheSize(l)) != cacheMap->end());
}

template <typename T>
mapCache<T> cacheInitT (const uint_fast8_t min, const uint_fast8_t max){
    mapCache<T> cache;
    T bits, nbits;
    uint_fast8_t i, cl;
    bientropy precalc, n_precalc;
    if (min < 2 || max > sizeof(T)*CHAR_BIT)
        return cache;

    for (i = min; i <= max; ++i)
    { // precache various length
        
        cl = cacheSize(i);
        precalc = { 0, 0, 0 };
        n_precalc = { 0, 0, 0 };
        
        for (bits = 0; bits < (1 << cl); bits += 2) //precache half of the variants
        {
            bienTtoStepT<T>(bits, &precalc, cl, 1, i - cl); //precache partial calculation
            nbits = trim_bitsT<T>(~bits, cl);
            bienTtoStepT<T>(nbits, &n_precalc, cl, 1, i - cl); //precache partial calculation for not

            cache.insert(std::make_pair(Key<T>(bits, cl, i), precalc)); //insert main
            if (n_precalc.bien != precalc.bien)
                cache.insert(std::make_pair(Key<T>(nbits, cl, i), n_precalc));//insert mirror if unsimmetrical
        
        }
    }
    return cache;
}

template <typename T>
bientropy getBientropyT(const mapCache<T>* cache, T bits, const uint_fast8_t length) {
    if (cache == NULL || !cacheContains<T>(cache, length))
        return bienTs<T>(bits, length, 1);
    else 
        return bienTcached<T>(bits, length, cacheSize(length), &cache_callback, (void*)cache); //using lookup table
}



#endif