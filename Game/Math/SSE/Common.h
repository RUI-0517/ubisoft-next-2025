#pragma once
#include <xmmintrin.h>

#if defined(__SSE4_1__) || defined(__AVX__) || defined(__AVX2__)
#include <smmintrin.h>
#elif defined(__SSE2__) || defined(_M_IX86) || defined(_M_X64)
#include <emmintrin.h>
#else
    #error Unsupported platform
#endif

template <size_t N, typename T, typename Enable = void>
struct Vector;
