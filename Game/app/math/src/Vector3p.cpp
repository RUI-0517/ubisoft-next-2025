#include <stdafx.h>
#include "../include/Vector3p.h"

Vector3p Vector3p::operator+(const Vector3p& other) const
{
	return Vector3p(_mm_add_ps(m_value, other.m_value));
}

Vector3p& Vector3p::operator+=(const Vector3p& other)
{
	return *this = *this + other;
}

Vector3p Vector3p::operator-(const Vector3p& other) const
{
	return Vector3p(_mm_sub_ps(m_value, other.m_value));
}

Vector3p& Vector3p::operator-=(const Vector3p& other)
{
	return *this = *this - other;
}

Vector3p Vector3p::operator*(const float scalar) const
{
	const __m128 packedScalar = _mm_set1_ps(scalar);
	return Vector3p(_mm_mul_ps(m_value, packedScalar));
}

Vector3p& Vector3p::operator*=(const float scalar)
{
	return *this = *this * scalar;
}

Vector3p Vector3p::operator/(const float scalar) const
{
	const __m128 packedScalar = _mm_set1_ps(scalar);
	return Vector3p(_mm_div_ps(m_value, packedScalar));
}

Vector3p& Vector3p::operator/=(const float scalar)
{
	return *this = *this / scalar;
}

bool Vector3p::operator==(const Vector3p& other) const
{
	return true;
}

bool Vector3p::operator!=(const Vector3p& other) const
{
	return false;
}

float Vector3p::dot(const Vector3p& other) const
{
	return 0;
}

Vector3p Vector3p::cross(const Vector3p& other) const
{
	return {};
}

float Vector3p::magnitude() const
{
	return 0;
}

float Vector3p::magnitudeSquared() const
{
	return 0;
}

Vector3p Vector3p::normalize() const
{
	return {};
}

float Vector3p::distance(const Vector3p& other) const
{
	return 0;
}

float Vector3p::distanceSquared(const Vector3p& other) const
{
	return 0;
}

Vector3p Vector3p::lerp(const Vector3p& other, float t) const
{
	return {};
}

Vector3p Vector3p::hadamard(const Vector3p& other) const
{
	return {};
}

Vector3p Vector3p::clamp(float minLength, float maxLength) const
{
	return {};
}

// float& Vector3p::operator[](size_t index)
// {
// 	return ;
// }
//
// const float& Vector3p::operator[](size_t index) const
// {
// 	
// 	return 0;
// }

std::ostream& operator<<(std::ostream& os, const Vector3p& vector)
{
	return os;
}

Vector3p::Vector3p()
{
	m_value = _mm_setzero_ps();
}

Vector3p::Vector3p(const float x, const float y, const float z)
{
	alignas(16) const float temp[4] = {x, y, z, 0};
	m_value = _mm_loadr_ps(temp);
}

Vector3p::Vector3p(const std::initializer_list<float> elements)
{
	const float* begin = elements.begin();
	alignas(16) const float temp[4] = {*begin++, *begin++, *begin, 0};
	m_value = _mm_loadr_ps(temp);
}

Vector3p::Vector3p(const __m128 value): m_value(value)
{
}

Vector3p::~Vector3p() = default;

Vector3p::Vector3p(const Vector3p& other): m_value()
{
}

Vector3p& Vector3p::operator=(const Vector3p& other)
{
	return *this;
}

Vector3p::Vector3p(Vector3p&& other) noexcept: m_value()
{
}

Vector3p& Vector3p::operator=(Vector3p&& other) noexcept
{
	return *this;
}
