#pragma once

#include <cassert>

// Toggle to disable/enable simd manually
#define USE_SIMD

#if defined(USE_SIMD) && (defined(__AVX__) || defined(__AVX2__) || defined(_M_IX86) || defined(_M_X64))
#include "SSE/Matrix.h"
#else
// #include "Scalar/Vector.h"
#endif
