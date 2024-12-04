#include <stdafx.h>
#include "../include/Vector3p.h"

float& Vector3p::operator[](const size_t index)
{
	assert(index > 0 && index <3 && "Index out of bounds");
	return (&x)[index];
}

const float& Vector3p::operator[](const size_t index) const
{
	assert(index > 0 && index < 3 && "Index out of bounds");
	return (&x)[index];
}

Vector3p::Vector3p() = default;

Vector3p::Vector3p(const float _x, const float _y, const float _z): Vector4p(_x, _y, _z, 0.0f)
{
}

Vector3p::Vector3p(const std::initializer_list<float> elements)
{
	const float* begin = elements.begin();

	const float x = *begin++;
	const float y = *begin++;
	const float z = *begin;

	m_value = _mm_setr_ps(x, y, z, 0.0f);
}

Vector3p::Vector3p(const __m128 value)
{
	m_value = value;
}

Vector3p::Vector3p(const Vector4p& other)
{
	m_value = other.m_value;
}

Vector3p::~Vector3p() = default;

Vector3p::Vector3p(const Vector3p& other) = default;

Vector3p& Vector3p::operator=(const Vector3p& other) = default;

Vector3p::Vector3p(Vector3p&& other) noexcept = default;

Vector3p& Vector3p::operator=(Vector3p&& other) noexcept = default;

std::ostream& Vector3p::to_string_impl(std::ostream& os, const Vector4p& vector) const
{
	os << "[" << vector[0] << ' ' << vector[1] << ' ' << vector[2] << "]";
	return os;
}
