#include <stdafx.h>
#include "../include/Vector4p.h"

Vector4p Vector4p::operator+(const Vector4p& other) const
{
	return Vector4p(_mm_add_ps(m_value, other.m_value));
}

Vector4p& Vector4p::operator+=(const Vector4p& other)
{
	return *this = *this + other;
}

Vector4p Vector4p::operator-(const Vector4p& other) const
{
	return Vector4p(_mm_sub_ps(m_value, other.m_value));
}

Vector4p& Vector4p::operator-=(const Vector4p& other)
{
	return *this = *this - other;
}

Vector4p Vector4p::operator*(const float scalar) const
{
	const __m128 packedScalar = _mm_set1_ps(scalar);
	return Vector4p(_mm_mul_ps(m_value, packedScalar));
}

Vector4p& Vector4p::operator*=(const float scalar)
{
	return *this = *this * scalar;
}

Vector4p Vector4p::operator/(const float scalar) const
{
	const float inverseScalar = 1.0f / scalar;
	const __m128 packedInverseScalar = _mm_set1_ps(inverseScalar);
	return Vector4p(_mm_mul_ps(m_value, packedInverseScalar));;
}

Vector4p& Vector4p::operator/=(const float scalar)
{
	return *this = *this / scalar;
}

bool Vector4p::operator==(const Vector4p& other) const
{
	const __m128 cmp = _mm_cmpeq_ps(this->m_value, other.m_value);
	const int mask = _mm_movemask_ps(cmp);
	return mask == 0xF;
}

bool Vector4p::operator!=(const Vector4p& other) const
{
	return !(*this == other);
}

// The function involving _mm_cvtss_f32 appears 
// to be slower than its scalar counterpart.
float Vector4p::dot(const Vector4p& other) const
{
	return dot_impl_scalar(other.m_value);
}

// TODO: Optimize
Vector4p Vector4p::cross(const Vector4p& other) const
{
	// Cross product formula:
	// a[1] * b[2] - a[2] * b[1]
	// a[2] * b[0] - a[0] * b[2]
	// a[0] * b[1] - a[1] * b[0]

	// Shuffle for the first part of the formula:
	// For vector a: {3, 0, 2, 1}, For vector b: {3, 1, 0, 2}
	// The result of multiplying these will give the left side of the cross product.
	// Note: In the _MM_SHUFFLE macro, the index counts from right to left.
	const __m128 tmp0 = _mm_shuffle_ps(m_value, m_value, _MM_SHUFFLE(3, 0, 2, 1));
	const __m128 tmp1 = _mm_shuffle_ps(other.m_value, other.m_value, _MM_SHUFFLE(3, 1, 0, 2));

	// Shuffle for the second part of the formula:
	// This uses the same shuffle logic as before.
	const __m128 tmp2 = _mm_shuffle_ps(m_value, m_value, _MM_SHUFFLE(3, 1, 0, 2));
	const __m128 tmp3 = _mm_shuffle_ps(other.m_value, other.m_value, _MM_SHUFFLE(3, 0, 2, 1));

	// Subtract the two results to complete the cross product calculation.
	return Vector4p{
		_mm_sub_ps(
			_mm_mul_ps(tmp0, tmp1),
			_mm_mul_ps(tmp2, tmp3)
		)
	};
}

float Vector4p::magnitude() const
{
	return _mm_cvtss_f32(_mm_sqrt_ps(dot_impl(m_value)));
}

float Vector4p::magnitudeSquared() const
{
	return _mm_cvtss_f32(dot_impl(m_value));
}

Vector4p Vector4p::normalize() const
{
	return Vector4p(_mm_div_ps(m_value, _mm_sqrt_ps(dot_impl(m_value))));
}

float Vector4p::distance(const Vector4p& other) const
{
	return (*this - other).magnitude();
}

float Vector4p::distanceSquared(const Vector4p& other) const
{
	return (*this - other).magnitudeSquared();
}

Vector4p Vector4p::lerp(const Vector4p& other, const float t) const
{
	return *this * (1 - t) + other * t;
}

Vector4p Vector4p::hadamard(const Vector4p& other) const
{
	return Vector4p(_mm_mul_ps(m_value, other.m_value));
}

Vector4p Vector4p::clamp(const float minLength, const float maxLength) const
{
	const float mag = magnitude();
	if (mag < minLength) return normalize() * minLength;
	if (mag > maxLength) return normalize() * maxLength;
	return *this;
}

float& Vector4p::operator[](const size_t index)
{
	assert(index > 0 && index < 4 && "Index out of bounds");
	return (&x)[index];
}

const float& Vector4p::operator[](const size_t index) const
{
	assert(index > 0 && index < 4 && "Index out of bounds");
	return (&x)[index];
}

std::ostream& Vector4p::to_string_impl(std::ostream& os, const Vector4p& vector) const
{
	os << "[" << vector[0] << ' ' << vector[1] << ' ' << vector[2] << ' ' << vector[3] << "]";
	return os;
}

__m128 Vector4p::dot_impl(__m128 other) const
{
	// Dot product formula
	// {a.x * b.x + a.y * b.y + a.z * b.z
	// {ab.x + ab.y + ab.z}
	// {ab.xyz}
#if defined(__SSE4_1__) || defined(__AVX__) || defined(__AVX2__)
	// Hardware Implementation
    const __m128 result = _mm_dp_ps(m_value, other, 0xff);
#elif defined(__SSE2__) || defined(_M_IX86) || defined(_M_X64)
	// {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w}
	// {ab.x, ab.y, ab.z, ab.w}
	__m128 temp0 = _mm_mul_ps(m_value, other);
	// Note: In the _MM_SHUFFLE macro, the index counts from right to left.
	// {ab.y, ab.x, ab.w, ab.z}
	__m128 temp1 = _mm_shuffle_ps(temp0, temp0, _MM_SHUFFLE(2, 3, 0, 1));
	// {ab.x + ab.y, ab.y + ab.x, ab.z + ab.w, ab.w + ab.z}
	// {ab.xy, ab.yx, ab.zw, ab.wz}
	// {ab.xy, ab.xy, ab.zw, ab.zw}
	temp0 = _mm_add_ps(temp0, temp1);
	// {ab.zw, ab.zw, ab.xy, ab.xy}
	temp1 = _mm_shuffle_ps(temp0, temp0, _MM_SHUFFLE(0, 1, 2, 3));
	// {ab.xy + ab.zw, ab.xy + ab.zw, ab.zw + ab.xy, ab.zw + ab.xy}
	// {ab.xyzw, ab.xyzw, ab.xyzw, ab.xyzw}
	const __m128 result = _mm_add_ps(temp0, temp1);
#else
    #error Unsupported platform
#endif
	return result;
}

float Vector4p::dot_impl_scalar(const __m128 other) const
{
	return _mm_cvtss_f32(dot_impl(other));
}

std::ostream& operator<<(std::ostream& os, const Vector4p& vector)
{
	return vector.to_string_impl(os, vector);;
}

Vector4p::Vector4p(): m_value(_mm_setzero_ps())
{
}

Vector4p::Vector4p(const float x, const float y, const float z, const float w): m_value(_mm_setr_ps(x, y, z, w))
{
}

Vector4p::Vector4p(const std::initializer_list<float> elements)
{
	const float* begin = elements.begin();

	const float x = *begin++;
	const float y = *begin++;
	const float z = *begin++;
	const float w = *begin;

	m_value = _mm_setr_ps(x, y, z, w);
}

Vector4p::Vector4p(const __m128 value): m_value(value)
{
}

Vector4p::~Vector4p() = default;

Vector4p::Vector4p(const Vector4p& other) = default;

Vector4p& Vector4p::operator=(const Vector4p& other) = default;

Vector4p::Vector4p(Vector4p&& other) noexcept = default;

Vector4p& Vector4p::operator=(Vector4p&& other) noexcept = default;
