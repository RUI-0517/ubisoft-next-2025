#pragma once
#define ENABLE_SIMD

#ifdef ENABLE_SIMD
#include <xmmintrin.h>
#endif
#include <cassert>

#include "Vector3.h"

static Vector3 operator+(const Vector3& lhs, const Vector3& rhs)
{
#ifndef ENABLE_SIMD
	Vector3 result;
	for (int i = 0; i < 3; ++i)
		result[i] = lhs[i] + rhs[i];
	return result;
#else
	const __m128 a = _mm_load_ps(&lhs[0]);
	const __m128 b = _mm_load_ps(&rhs[0]);

	float result[4];
	_mm_store_ps(result, _mm_add_ps(a, b));

	return Vector3(result);
#endif
}

static Vector3 operator-(const Vector3& lhs, const Vector3& rhs)
{
#ifndef ENABLE_SIMD
	Vector3 result;
	for (int i = 0; i < 3; ++i)
		result[i] = lhs[i] - rhs[i];
	return result;
#else
	const __m128 a = _mm_load_ps(&lhs[0]);
	const __m128 b = _mm_load_ps(&rhs[0]);

	float result[4];
	_mm_store_ps(result, _mm_sub_ps(a, b));

	return Vector3(result);
#endif
}

 static Vector3 operator*(const Vector3& lhs, const float scalar)
 {
 #ifndef ENABLE_SIMD
 	Vector3 result;
 	for (int i = 0; i < 3; ++i)
 		result[i] = lhs[i] * scalar;
 	return result;
 #else
 	const __m128 a = _mm_load_ps(&lhs[0]);
 	const __m128 b = _mm_set1_ps(scalar);

 	float result[4];
 	_mm_store_ps(result, _mm_mul_ps(a, b));

 	return Vector3(result);
 #endif
 }

 static Vector3 operator/(const Vector3& lhs, const float scalar)
 {
 	assert(scalar != 0.0f && "Division by zero error in Vector3::operator/.");

 #ifndef ENABLE_SIMD
 	Vector3 result;
 	for (int i = 0; i < 3; ++i)
 		result[i] = lhs[i] / scalar;
 	return result;
 #else
 	const __m128 a = _mm_load_ps(&lhs[0]);
 	const __m128 b = _mm_set1_ps(scalar);

 	float result[4];
 	_mm_store_ps(result, _mm_div_ps(a, b));

 	return Vector3(result);
 #endif
 }
