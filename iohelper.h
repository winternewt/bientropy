#pragma once
#ifndef IOHELPER_H
#define IOHELPER_H

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

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

#ifdef __cplusplus
#include <chrono>
#include <ctime>
#include <iostream>
typedef std::chrono::high_resolution_clock hclock;
typedef hclock::time_point ts;
typedef std::chrono::duration<double> duration;

#else
#include <time.h>

#endif

#define TEST_HEX_NAME "input.hex"

#ifdef __GNUC__
template <class T>
__attribute__((always_inline)) inline void DoNotOptimize(const T& value) {
	asm volatile("" : "+m"(const_cast<T&>(value)));
}
#else

template <class T>
inline void DoNotOptimize(const T& value) {
	_ReadWriteBarrier();
}

#ifndef __cplusplus
#include <windows.h>
#define CLOCK_REALTIME 1
#define CLOCK_MONOTONIC 2

#define MS_PER_SEC      1000ULL     // MS = milliseconds
#define US_PER_MS       1000ULL     // US = microseconds
#define HNS_PER_US      10ULL       // HNS = hundred-nanoseconds (e.g., 1 hns = 100 ns)
#define NS_PER_US       1000ULL

#define HNS_PER_SEC     (MS_PER_SEC * US_PER_MS * HNS_PER_US)
#define NS_PER_HNS      (100ULL)    // NS = nanoseconds
#define NS_PER_SEC      (MS_PER_SEC * US_PER_MS * NS_PER_US)

int clock_gettime_monotonic(struct timespec* tv)
{
	static LARGE_INTEGER ticksPerSec;
	LARGE_INTEGER ticks;
	double seconds;

	if (!ticksPerSec.QuadPart) {
		QueryPerformanceFrequency(&ticksPerSec);
		if (!ticksPerSec.QuadPart) {
			errno = ENOTSUP;
			return -1;
		}
	}

	QueryPerformanceCounter(&ticks);

	seconds = (double)ticks.QuadPart / (double)ticksPerSec.QuadPart;
	tv->tv_sec = (time_t)seconds;
	tv->tv_nsec = (long)((ULONGLONG)(seconds * NS_PER_SEC) % NS_PER_SEC);

	return 0;
}

int clock_gettime_realtime(struct timespec* tv)
{

	FILETIME ft;
	ULARGE_INTEGER hnsTime;

	GetSystemTimeAsFileTime(&ft);

	hnsTime.LowPart = ft.dwLowDateTime;
	hnsTime.HighPart = ft.dwHighDateTime;

	// To get POSIX Epoch as baseline, subtract the number of hns intervals from Jan 1, 1601 to Jan 1, 1970.
	hnsTime.QuadPart -= (11644473600ULL * HNS_PER_SEC);

	// modulus by hns intervals per second first, then convert to ns, as not to lose resolution
	tv->tv_nsec = (long)((hnsTime.QuadPart % HNS_PER_SEC) * NS_PER_HNS);
	tv->tv_sec = (long)(hnsTime.QuadPart / HNS_PER_SEC);

	return 0;
}

int clock_gettime(int type, struct timespec* tp)
{
	if (type == CLOCK_MONOTONIC)
	{
		return clock_gettime_monotonic(tp);
	}
	else if (type == CLOCK_REALTIME)
	{
		return clock_gettime_realtime(tp);
	}

	errno = ENOTSUP;
	return -1;
}

#endif

#endif
typedef void (*f_progress)(const size_t, const size_t, void*); //callback 

inline void printnow() {
#ifndef __cplusplus
	//Структуры для сохранения определенного времени
	struct timespec mt1, mt2;
	//Переменная для расчета дельты времени
	long int tt;

	//Определяем текущее время
	clock_gettime(CLOCK_REALTIME, &mt1);

	//Выводим определенное время на экран консоли
	printf(“Секунды: % ld\n”, mt1.tv_sec);
	printf(“Наносекунды: % ld\n”, mt1.tv_nsec);

	//Определяем текущее время
	clock_gettime(CLOCK_REALTIME, &mt2);

	//Рассчитываем разницу времени между двумя измерениями
	tt = 1000000000 * (mt2.tv_sec - mt1.tv_sec) + (mt2.tv_nsec - mt1.tv_nsec);

	//Выводим результат расчета на экран
	printf(“Затрачено время : % ld нс\n”, tt);
#else
	auto time_point = std::chrono::system_clock::now();
	std::time_t now_c = std::chrono::system_clock::to_time_t(time_point); 
	std::cout << "===========" << std::endl;
	std::cout << "Now is " << std::ctime(&now_c);
	std::cout << "===========" << std::endl;
#endif
}


inline size_t progressbar(const size_t begin, const size_t end_inc, const size_t bar_size, f_progress foo, void* ctx)
{
	volatile char bar = '+'; //prevent optimizing, o3 is hardcore
	size_t it = end_inc - begin;
	size_t itd = it / bar_size;
	size_t i, c=0;
	printf("%c", '[');
	for (i = begin; itd>0 && i <= end_inc; i+=itd)
	{
		foo(i, i + itd-1, ctx);
		printf("%c", bar);
		if (++c >= bar_size) //owerflow failsafe
			break;
	}
	foo(begin+itd*bar_size, end_inc, ctx);
	for (; c < bar_size; c++)
	{
		printf("%c", bar);
	}
	printf("%c%c", ']','\n');
	return itd * bar_size + it % bar_size;
}


inline void printBits(const size_t size, void const* const ptr)
{
	unsigned char* b = (unsigned char*)ptr;
	unsigned char byte;
	long long int i, j;

	for (i = size - 1; i >= 0; i--) {
		for (j = CHAR_BIT - 1; j >= 0; j--) {
			byte = (b[i] >> j) & 1;
			printf("%u", byte);
		}
	}

}

inline void printLowerBits(const size_t bits, void const* const ptr)
{
	unsigned char* b = (unsigned char*)ptr;
	unsigned char byte;
	long long int i, j, l;
	l = (bits / CHAR_BIT) - 1;
	if (bits % CHAR_BIT > 0)
		l++;
	for (i = l; i >= 0; i--) {
		for (j = (i == l ? (bits - 1) % CHAR_BIT : CHAR_BIT - 1); j >= 0; j--) {
			byte = (b[i] >> j) & 1;
			printf("%u", byte);
		}
	}

}

inline unsigned char hex2int(const char input)
{
	if (input >= '0' && input <= '9')
		return input - '0';
	if (input >= 'A' && input <= 'F')
		return input - 'A' + 10;
	if (input >= 'a' && input <= 'f')
		return input - 'a' + 10;
	return 0;
}

unsigned char* getHEX(const unsigned long hexSize)
{
	//const char* TEST_HEX_NAME = "./4_entropydump.hex";
	// Simulated QRNG poll example, real ANU rng output loaded from file
	unsigned char* myHexString = (unsigned char*) malloc(hexSize);
	if (myHexString == NULL)
		return NULL;
	FILE* testFile = fopen(TEST_HEX_NAME, "r");
	if (testFile != NULL) {
		size_t res = fread((void*)myHexString, sizeof(char), hexSize, testFile);
		fclose(testFile);
		assert(res);
	}
	return myHexString;
};

char* getBinaryFromHEX(const unsigned long bytesSize) {

	char* myByteString = (char*) malloc(bytesSize);
	unsigned char* myHexString = (unsigned char*) getHEX(bytesSize * 2);
	if (myByteString == NULL || myHexString == NULL)
		return NULL;
	
	for (unsigned long i = 0; i < bytesSize; i++)
	{
		myByteString[i] = hex2int(myHexString[2*i]) * 16 + hex2int(myHexString[2 * i + 1]);
	
	}
	free(myHexString);//deallocate	
	return myByteString;
}
#endif
