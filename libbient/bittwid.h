#pragma once
#ifndef BITTWID_H
#define BITTWID_H
#include <cstdint>
#include <type_traits>
/*
 * Copyright (c) 2021 Newton Winter
 * inplementation of bit twiddling hacks collected by Sean Eron Anderson © 1997-2005 https://graphics.stanford.edu/~seander/bithacks.html
 * 
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

 //todo: intrinsic checks
 // https://stackoverflow.com/questions/6121792/how-to-check-if-a-cpu-supports-the-sse3-instruction-set
#define BUILT_IN

#ifdef __GNUC__
    #if __x86_64__ || __ppc64__
        #define IS64BIT
    #else
        #define IS32BIT
    #endif
#endif
#ifdef _MSC_VER
    #ifdef BUILT_IN
        #include <intrin.h>
    #endif
    #if _WIN64
        #define IS64BIT
    #else
        #define IS32BIT
    #endif
#endif

static const int powers2[] = { 1, 2, 4, 8, 16, 32, 64, 128, 256 }; 
static const int binaryMagicNumbers[5] = { 0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF, 0x0000FFFF }; // Magic Binary Numbers
static const uint_fast8_t BitsSetTable256[256] = //On July 14, 2009 Hallvard Furuseth suggested the macro compacted table.
{
#   define B2(n) n,     n+1,     n+1,     n+2
#   define B4(n) B2(n), B2(n+1), B2(n+1), B2(n+2)
#   define B6(n) B4(n), B4(n+1), B4(n+1), B4(n+2)
    B6(0), B6(1), B6(1), B6(2)
};

static const uint_fast16_t MortonTable256[256] =
{
  0x0000, 0x0001, 0x0004, 0x0005, 0x0010, 0x0011, 0x0014, 0x0015,
  0x0040, 0x0041, 0x0044, 0x0045, 0x0050, 0x0051, 0x0054, 0x0055,
  0x0100, 0x0101, 0x0104, 0x0105, 0x0110, 0x0111, 0x0114, 0x0115,
  0x0140, 0x0141, 0x0144, 0x0145, 0x0150, 0x0151, 0x0154, 0x0155,
  0x0400, 0x0401, 0x0404, 0x0405, 0x0410, 0x0411, 0x0414, 0x0415,
  0x0440, 0x0441, 0x0444, 0x0445, 0x0450, 0x0451, 0x0454, 0x0455,
  0x0500, 0x0501, 0x0504, 0x0505, 0x0510, 0x0511, 0x0514, 0x0515,
  0x0540, 0x0541, 0x0544, 0x0545, 0x0550, 0x0551, 0x0554, 0x0555,
  0x1000, 0x1001, 0x1004, 0x1005, 0x1010, 0x1011, 0x1014, 0x1015,
  0x1040, 0x1041, 0x1044, 0x1045, 0x1050, 0x1051, 0x1054, 0x1055,
  0x1100, 0x1101, 0x1104, 0x1105, 0x1110, 0x1111, 0x1114, 0x1115,
  0x1140, 0x1141, 0x1144, 0x1145, 0x1150, 0x1151, 0x1154, 0x1155,
  0x1400, 0x1401, 0x1404, 0x1405, 0x1410, 0x1411, 0x1414, 0x1415,
  0x1440, 0x1441, 0x1444, 0x1445, 0x1450, 0x1451, 0x1454, 0x1455,
  0x1500, 0x1501, 0x1504, 0x1505, 0x1510, 0x1511, 0x1514, 0x1515,
  0x1540, 0x1541, 0x1544, 0x1545, 0x1550, 0x1551, 0x1554, 0x1555,
  0x4000, 0x4001, 0x4004, 0x4005, 0x4010, 0x4011, 0x4014, 0x4015,
  0x4040, 0x4041, 0x4044, 0x4045, 0x4050, 0x4051, 0x4054, 0x4055,
  0x4100, 0x4101, 0x4104, 0x4105, 0x4110, 0x4111, 0x4114, 0x4115,
  0x4140, 0x4141, 0x4144, 0x4145, 0x4150, 0x4151, 0x4154, 0x4155,
  0x4400, 0x4401, 0x4404, 0x4405, 0x4410, 0x4411, 0x4414, 0x4415,
  0x4440, 0x4441, 0x4444, 0x4445, 0x4450, 0x4451, 0x4454, 0x4455,
  0x4500, 0x4501, 0x4504, 0x4505, 0x4510, 0x4511, 0x4514, 0x4515,
  0x4540, 0x4541, 0x4544, 0x4545, 0x4550, 0x4551, 0x4554, 0x4555,
  0x5000, 0x5001, 0x5004, 0x5005, 0x5010, 0x5011, 0x5014, 0x5015,
  0x5040, 0x5041, 0x5044, 0x5045, 0x5050, 0x5051, 0x5054, 0x5055,
  0x5100, 0x5101, 0x5104, 0x5105, 0x5110, 0x5111, 0x5114, 0x5115,
  0x5140, 0x5141, 0x5144, 0x5145, 0x5150, 0x5151, 0x5154, 0x5155,
  0x5400, 0x5401, 0x5404, 0x5405, 0x5410, 0x5411, 0x5414, 0x5415,
  0x5440, 0x5441, 0x5444, 0x5445, 0x5450, 0x5451, 0x5454, 0x5455,
  0x5500, 0x5501, 0x5504, 0x5505, 0x5510, 0x5511, 0x5514, 0x5515,
  0x5540, 0x5541, 0x5544, 0x5545, 0x5550, 0x5551, 0x5554, 0x5555
};

const uint64_t deBruijnMagic32 = 0x077CB531U;
static const uint_fast16_t MultiplyDeBruijnBitPosition32[32] = {
    0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
    31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
};

const uint64_t deBruijnMagic64 = 0x03f79d71b4cb0a89U;
static const uint_fast16_t MultiplyDeBruijnBitPosition64[64] = {
    0,  1, 48,  2, 57, 49, 28,  3,
    61, 58, 50, 42, 38, 29, 17,  4,
    62, 55, 59, 36, 53, 51, 43, 22,
    45, 39, 33, 30, 24, 18, 12,  5,
    63, 47, 56, 27, 60, 41, 37, 16,
    54, 35, 52, 21, 44, 32, 23, 11,
    46, 26, 40, 15, 34, 20, 31, 10,
    25, 14, 19,  9, 13,  8,  7,  6
};


//************************** Bit Counting (population functions) **************************

/*! @fn uint32_t pop_lookup(const char v)
 *  @brief      Counting bits set by lookup table 
 *  @param      v value
 *  @returns    population count(v) 
 */
inline uint32_t pop_lookup(const uint32_t v) {
   // Option 1:
    return BitsSetTable256[v & 0xff] +
        BitsSetTable256[(v >> 8) & 0xff] +
        BitsSetTable256[(v >> 16) & 0xff] +
        BitsSetTable256[v >> 24];
}

/*! @fn uint32_t pop64_32max(const uint32_t v)
 *  @brief      Counting bits set in 32-bit words using 64-bit instructions
 *  @param      v 32-bit value
 *  @returns    population count(v), 24 max
 */
uint32_t pop64_32max(const uint32_t v) {
    uint32_t c;     // option 3, for at most 32-bit values in v:
    c = ((v & 0xfff) * 0x1001001001001ULL & 0x84210842108421ULL) % 0x1f;
    c += (((v & 0xfff000) >> 12) * 0x1001001001001ULL & 0x84210842108421ULL) %
        0x1f;
    c += ((v >> 24) * 0x1001001001001ULL & 0x84210842108421ULL) % 0x1f;
    return c;
}

/*! @fn uint32_t pop32(const uint32_t v)
 *  @brief      Count the number of set bits in v.
 *  @param      v 64-bit value
 *  @returns    population count(v), 64 max
 */
inline uint32_t pop(const uint32_t v) {
#ifdef BUILT_IN
    #ifdef __GNUC__
        return __builtin_popcount(v); //__popcnt available through <intrin.h>
    #else 
        return __popcnt(v);
    #endif
#else
    uint32_t c = v - ((v >> 1) & 0x55555555);       
    c = (c & 0x33333333) + ((c >> 2) & 0x33333333);     
    return ((c + (c >> 4) & 0xF0F0F0F) * 0x1010101) >> 24; // count
#endif
}

/*! @fn uint32_t pop64(const uint64_t v)
 *  @brief      Count the number of set bits in v.
 *  @param      v 64-bit value
 *  @returns    population count(v), 64 max
 */
inline uint64_t pop(const uint64_t v) {
#if defined(BUILT_IN) && defined(IS64BIT)
    #ifdef __GNUC__
        return __builtin_popcountl(v); //__popcnt available through <intrin.h>
    #else 
        return __popcnt64(v);       
    #endif
#else
    uint64_t c = v - ((v >> 1) & 0x5555555555555555UL);                
    c = (c & 0x3333333333333333UL) + ((c >> 2) & 0x3333333333333333UL); // temp
    return (((c + (c >> 4)) & 0x0F0F0F0F0F0F0F0FUL) * 0x0101010101010101UL) >> 56; // count
#endif
}

/*! @fn uint32_t clz32(const uint32_t v) 
 *  @brief      Count the leading zero bits count of v
 *  @param      v 32-bit value
 *  @returns    clz(v), 32 max
 */
static uint32_t inline clz(const uint32_t v) {
#if defined(BUILT_IN)
#ifdef __GNUC__
    return __builtin_clz(v); //__lzcnt available through <intrin.h>
#else
    return __lzcnt(v);
    /*
    unsigned long leading_zero = 0;
    if (_BitScanReverse(&leading_zero, v))
    {
        return 31 - leading_zero;
    }
    else
        return 32;// Same remarks as above
    */
#endif
#else
    uint32_t c = v;
    c |= (c >> powers2[0]);
    c |= (c >> powers2[1]);
    c |= (c >> powers2[2]);
    c |= (c >> powers2[3]);
    c |= (c >> powers2[4]);
    return powers2[5] - pop32(c);
#endif
}

/*! @fn uint64_t clz64(const uint64_t v)
 *  @brief      Count the leading zero bits count of v
 *  @param      v 32-bit value
 *  @returns    clz(v), 32 max
 */
static uint64_t inline clz(const uint64_t v) {
#if defined(BUILT_IN) && defined(IS64BIT)
#ifdef __GNUC__
    return __builtin_clzl(v); //__lzcnt64 available through <intrin.h>
#else 
    return __lzcnt64(v);
#endif
#else
    uint64_t c = v;
    c |= (c >> powers2[0]);
    c |= (c >> powers2[1]);
    c |= (c >> powers2[2]);
    c |= (c >> powers2[3]);
    c |= (c >> powers2[4]);
    c |= (c >> powers2[5]);
    return powers2[6] - pop(c);
#endif
}

/*! @fn uint32_t pop64p(const uint64_t v)
 *  @brief      Count the number of set bits in v, in parallel 
 *  @param      v 64-bit value
 *  @returns    population count(v), 64 max
 */
inline uint64_t pop64p(const uint64_t v) {
    uint64_t c = v - ((v >> 1) & binaryMagicNumbers[0]);
    c = ((c >> powers2[1]) & binaryMagicNumbers[1]) + (c & binaryMagicNumbers[1]);
    c = ((c >> powers2[2]) + c) & binaryMagicNumbers[2];
    c = ((c >> powers2[3]) + c) & binaryMagicNumbers[3];
    return ((c >> powers2[4]) + c) & binaryMagicNumbers[4];
}


/*! @fn T pop64(const T v)
 *  @brief      A generalization of the best bit counting method to integers of bit - widths upto 128 (parameterized by type T) is this:
 *  @param      v value (parameterized by type T) 
 *  @returns    population count(v), 128 max
 */
template <typename T>
inline T popT(const T v) {
    if constexpr (std::is_same_v<T, uint32_t> || std::is_same_v<T, uint64_t>) //compile-time choice for intrinsics
        return pop(v);
    T c = v - ((v >> 1) & (T)~(T)0 / 3);                           // temp
    c = (c & (T)~(T)0 / 15 * 3) + ((c >> 2) & (T)~(T)0 / 15 * 3);      // temp
    c = (c + (c >> 4)) & (T)~(T)0 / 255 * 15;                      // temp
    return (T)(c * ((T)~(T)0 / 255)) >> (sizeof(T) - 1) * CHAR_BIT; // count
}

/************************** Other bitwise **************************/

/*! @fn uint32_t morton_lookup(const uint16_t x, const uint16_t y)
 *  @brief      Interleave bits by table lookup, Interleaved bits (aka Morton numbers) are useful for linearizing 2D integer coordinates.  
 *  @param      x, y 16 bit values
 *  @returns    32-bit Morton Number that can be compared easily and has the property that a number is usually close to another if their x and y values are close
 */
inline uint32_t morton_lookup(const uint16_t x, const uint16_t y) { // gets the resulting 32-bit Morton Number.
    return MortonTable256[y >> 8] << 17 |
           MortonTable256[x >> 8] << 16 |
           MortonTable256[y & 0xFF] << 1 |
           MortonTable256[x & 0xFF];
}

/*! @fn uint16_t morton_(const uint8_t x, const uint8_t y)
 *  @brief      In 11 operations, this version interleaves bits of two bytes (rather than shorts, as in the other versions), but many of the operations are 64-bit multiplies so it isn't appropriate for all machines. 
 *  @param      The input parameters, x and y, should be less than 256. 
 *  @returns    16-bit Morton Number that can be compared easily and has the property that a number is usually close to another if their x and y values are close
 */
inline uint16_t morton(const uint8_t x, const uint8_t y) { // gets the resulting 16-bit Morton Number.
    return (((x * 0x0101010101010101ULL & 0x8040201008040201ULL) *
        0x0102040810204081ULL >> 49) & 0x5555) | 
        (((y * 0x0101010101010101ULL & 0x8040201008040201ULL) *
            0x0102040810204081ULL >> 48) & 0xAAAA);
}

/*! @fn uint32_t log2(uint32_t v)
 *  @brief The code  computes the log base 2 of a 32-bit integer with a small table lookup and multiply.
 *  @param      v value
 *  @returns    log2(v) (rounded down)
 */
inline uint32_t log2i(uint32_t v) {
    v |= v >> powers2[0];
    v |= v >> powers2[1];
    v |= v >> powers2[2];
    v |= v >> powers2[3];
    v |= v >> powers2[4];
    v = (v >> 1) + 1;
    return MultiplyDeBruijnBitPosition32[((uint32_t)(v * deBruijnMagic32)) >> 27];
}

/*! @fn uint64_t log2(uint64_t v)
 *  @brief The code  computes the log base 2 of a 64-bit integer with a small table lookup and multiply.
 *  @param      v value
 *  @returns    log2(v) (rounded down)
 */
inline uint64_t log2i(uint64_t v) {
    v |= v >> powers2[0];
    v |= v >> powers2[1];
    v |= v >> powers2[2];
    v |= v >> powers2[3];
    v |= v >> powers2[4];
    v |= v >> powers2[5];
    v = (v >> 1) + 1;
    return MultiplyDeBruijnBitPosition64[((uint64_t)(v * deBruijnMagic64)) >> 58];
}
//https://stackoverflow.com/questions/757059/position-of-least-significant-bit-that-is-set
/*! @fn uint32_t findLSB1(uint32_t v)
 *  @brief      Fast, LUT based, calculation of the lowest non-zero bit in a 32-bit integer
 *  @param      v 32-bit value
 *  @returns    1-based bit-index of lowest non-zero bit
 */
inline uint32_t findLSB1(uint32_t v) { // returns 1-based index of first non-zero bit.
    if (!v) {
        return 0;
    }
    return MultiplyDeBruijnBitPosition32[((uint32_t) ((v ^ (v - 1)) * deBruijnMagic32)) >> 27] + 1;
}

/*! @fn uint32_t findLSB1(uint64_t v)
 *  @brief      Fast, LUT based, calculation of the lowest non-zero bit in a 64-bit integer
 *  @param      v 64-bit value
 *  @returns    1-based bit-index of lowest non-zero bit
 */
inline int findLSB1(uint64_t v) { // returns 1-based index of first non-zero bit.
    if (!v) {
        return 0;
    }
    return MultiplyDeBruijnBitPosition64[((uint64_t)((v ^ (v - 1)) * deBruijnMagic64)) >> 58] + 1;
}

/*! @name swapBits
   *  @brief      swap the bits set in "mask" between a and b
   *  @param      a Input
   *  @param      b Input
   *  @param      mask a bitmask specifying which bits to swap between a and b
   *  @param      n number of elements in input arrays
   */
//@{
template <typename T>
inline void swapBits(T& a, T& b, const T mask = 0xFFFFFFFFFFFFFFFF) {
    T masked = (a ^ b) & mask;
    a ^= masked;
    b ^= masked;
}

template <typename T>
void swapBits(T* a, T* b, const T mask, size_t n = 1) {
    for (size_t i = 0; i < n; ++i) {
        swapBits(*a++, *b++, mask);
    }
}    
//@}
/*! @fn         template <typename T> void flipBits(T* a, const T mask, size_t n = 1)
   *  @brief      flip the bits in a which are set in "mask"
   *  @param      a pointer to the data to be "flipped"
   *  @param      mask a bitmask specifying which bits to flip
   *  @param      n number of elements to flip
   */
template <typename T>
void flipBits(T* a, const T mask, size_t n = 1) {
    for (size_t i = 0; i < n; ++i) {
        *a++ ^= mask;
    }
}


#endif
