#pragma once
#ifndef LUT_CRAFT
#define LUT_CRAFT
#include <cstddef>
#include <climits>
#include <array>

/*
 * Copyright (c) 2021 Newton Winter
 * Dased on dyp's code https://stackoverflow.com/questions/19016099/lookup-table-with-constexpr
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

namespace LUT {
#define UNUSED(x) (void)(x)

    // by Xeo, from https://stackoverflow.com/a/13294458/420683      
    template<class Function, std::size_t... Indices>
    constexpr auto make_array_helper(Function f, std::index_sequence<Indices...>)
        ->std::array<typename std::result_of<Function(std::size_t, std::size_t)>::type, sizeof...(Indices)>
    {
        return { { f(Indices, sizeof...(Indices))... } };
    }

    template<int N, class Function>
    constexpr auto make_array(Function f)
        ->std::array<typename std::result_of<Function(std::size_t, std::size_t)>::type, N>
    {
        return make_array_helper(f, std::make_index_sequence<N>{});
    }

    template <typename T>
    constexpr T bitmask_gen(std::size_t curr, std::size_t total) // stackoverflow.com/questions/1392059/algorithm-to-generate-bit-mask 
    {
      UNUSED(total);
      return (curr != 0)
          ? (static_cast<T>(-1) >> ((sizeof(T) * CHAR_BIT) - curr))
          : 0;
    //    return static_cast<T>(-(curr != 0)) //msvc chokes on this
    //        & (static_cast<T>(-1) >> ((sizeof(T) * CHAR_BIT) - curr));
    
    }

    template <typename T>
    constexpr T bitset_gen(std::size_t curr, std::size_t total)
    {
	UNUSED(total);
        return (curr == 0) ? 0 : (curr & 1) + bitset_gen<T>(curr >> 1, total);
    }

    template <typename T>
    constexpr T powers2_gen(std::size_t curr, std::size_t total)
    {
	UNUSED(total);
        return (1ULL << curr);
    }
    
    template <typename T>
    constexpr long double powers2f_gen(std::size_t curr, std::size_t total)
    {
        UNUSED(total);
        return 2.0L* powers2_gen<T>(curr-1,total);
    }

    #if (__cplusplus >= 202002L)
    // c20 voodoo is fubar on msvc, so using lut//
    consteval long double log_gen(std::size_t curr, std::size_t total)
    {
        UNUSED(total);
        return (curr == 0) ? 0 : std::log2l(curr);
    }
    constexpr auto lLog2 = generate_array<sizeof(int64_t)* CHAR_BIT * 2>(log_gen<T>);
    #endif

    template <typename T>
    constexpr auto BitMasks = make_array<sizeof(T)* CHAR_BIT +1>(bitmask_gen<T>);

    template <typename T>
    constexpr auto BitSet = make_array<1 << (sizeof(T) * CHAR_BIT)>(bitset_gen<T>);

    template <typename T>
    constexpr auto Powers2 = make_array<sizeof(T)* CHAR_BIT + 1>(powers2_gen<T>);
    
    template <typename T>
    constexpr auto Powers2f = make_array<sizeof(T)* CHAR_BIT + 1>(powers2f_gen<T>);
    //template <typename T>
    //using Powers2sum = BitMasks<T>; //same
    //// consider https://github.com/Morwenn/static_math/tree/master/include/static_math

}

#endif
