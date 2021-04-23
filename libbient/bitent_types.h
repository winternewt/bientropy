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
#include <vector>

typedef double FloatT; //precision choice
typedef uint32_t Bits; //define bitstring

static const uint_fast16_t BIENTROPY_MINLENGTH = 2; //it's defined for bit len >= 2
static const uint_fast16_t BIENTROPY_MAXLENGTH = sizeof(Bits)*CHAR_BIT; //it's defined for bit len >= 2
static const uint_fast16_t CACHE_MAX_LENGTH = 25; //configure memory consumption
static const uint_fast16_t BITENT_STAT_LENGTH = 32;

typedef struct bientropy {
    FloatT bien; // BiEntropy
    FloatT tbien; // TBiEntropy
    FloatT t; //auxilary field, bitstring length and sign for last binary derivative: - is 0 and + is 1
} bientropy;

template <typename T>
class ThinCache;

#endif
