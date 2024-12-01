#pragma once

#ifdef ENABLE_SIMD
#include "Vector3p.h"
using Vector3 = Vector3p;
#else
#include "Vector3.h"
#endif // ENABLE_SIMD
