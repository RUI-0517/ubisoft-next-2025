#pragma once

#if defined(__SSE4_1__) || defined(__AVX__) || defined(__AVX2__)||defined(__SSE2__) || defined(_M_IX86) || defined(_M_X64)
#include "Vector3p.h"
using Vector3 = Vector3p;
#else
#include "Vector3.h"
#endif // ENABLE_SIMD
