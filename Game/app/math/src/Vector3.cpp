#include <stdafx.h>
#include "../include/Vector3.h"
#include <cassert>

Vector3 Vector3::operator+(const Vector3& other) const
{
	Vector3 result;
	for (int i = 0; i < 3; ++i)
		result[i] = (*this)[i] + other[i];
	return result;
}

Vector3& Vector3::operator+=(const Vector3& other)
{
	return *this = *this + other;
}

Vector3 Vector3::operator-(const Vector3& other) const
{
	Vector3 result;
	for (int i = 0; i < 3; ++i)
		result[i] = (*this)[i] - other[i];
	return result;
}

Vector3& Vector3::operator-=(const Vector3& other)
{
	return *this = *this - other;
}

Vector3 Vector3::operator*(const float scalar) const
{
	Vector3 result;
	for (int i = 0; i < 3; ++i)
		result[i] = (*this)[i] * scalar;
	return result;
}

Vector3& Vector3::operator*=(const float scalar)
{
	return *this = *this * scalar;
}

Vector3 Vector3::operator/(const float scalar) const
{
	assert(scalar != 0.0f && "Division by zero error in Vector3::operator/.");
	Vector3 result;
	for (int i = 0; i < 3; ++i)
		result[i] = (*this)[i] * (1 / scalar);
	return result;
}

Vector3& Vector3::operator/=(const float scalar)
{
	return *this = *this / scalar;
}

bool Vector3::operator==(const Vector3& other) const
{
	return std::abs((*this)[0] - other[0]) < 1e-5 &&
		std::abs((*this)[1] - other[1]) < 1e-5 &&
		std::abs((*this)[2] - other[2]) < 1e-5;
}

bool Vector3::operator!=(const Vector3& other) const
{
	return !(*this == other);
}

float Vector3::dot(const Vector3& other) const
{
	return (*this)[0] * other[0] + (*this)[1] * other[1] + (*this)[2] * other[2];
}

Vector3 Vector3::cross(const Vector3& other) const
{
	return {
		(*this)[1] * other[2] - (*this)[2] * other[1],
		(*this)[2] * other[0] - (*this)[0] * other[2],
		(*this)[0] * other[1] - (*this)[1] * other[0]
	};
}

float Vector3::magnitude() const
{
	return std::sqrt(dot(*this));
}

float Vector3::magnitudeSquared() const
{
	return dot(*this);
}

Vector3 Vector3::normalize() const
{
	const float mag = magnitude();
	return (mag > 0) ? *this / mag : Vector3(0, 0, 0);
}

float Vector3::distance(const Vector3& other) const
{
	return (*this - other).magnitude();
}

float Vector3::distanceSquared(const Vector3& other) const
{
	return (*this - other).magnitudeSquared();
}

Vector3 Vector3::lerp(const Vector3& other, const float t) const
{
	return *this * (1 - t) + other * t;
}

Vector3 Vector3::hadamard(const Vector3& other) const
{
	return {(*this)[0] * other[0], (*this)[1] * other[1], (*this)[2] * other[2]};
}

Vector3 Vector3::clamp(const float minLength, const float maxLength) const
{
	const float mag = magnitude();
	if (mag < minLength) return normalize() * minLength;
	if (mag > maxLength) return normalize() * maxLength;
	return *this;
}

std::ostream& operator<<(std::ostream& os, const Vector3& vector)
{
	os << "[" << vector[0] << ' ' << vector[1] << ' ' << vector[2] << "]";
	return os;
}

Vector3::Vector3()
{
	std::fill_n(m_data, 3, 0.0f);
}

Vector3::Vector3(const float x, const float y, const float z)
{
	(*this)[0] = x;
	(*this)[1] = y;
	(*this)[2] = z;
}

Vector3::Vector3(const std::initializer_list<float> elements)
{
	std::copy_n(elements.begin(), 3, m_data);
}

Vector3::~Vector3() = default;

Vector3::Vector3(const Vector3& other)
{
	std::copy_n(other.m_data, 3, m_data);
}

Vector3& Vector3::operator=(const Vector3& other)
{
	if (this != &other) std::copy_n(other.m_data, 3, m_data);
	return *this;
}

Vector3::Vector3(Vector3&& other) noexcept
{
	std::copy_n(other.m_data, 3, m_data);
}

Vector3& Vector3::operator=(Vector3&& other) noexcept
{
	std::copy_n(other.m_data, 3, m_data);
	return *this;
}

float& Vector3::operator[](const size_t index)
{
	assert(index > 0 && index < 3 && "Index out of bounds");
	return m_data[index];
}

const float& Vector3::operator[](const size_t index) const
{
	assert(index > 0 && index < 3 && "Index out of bounds");
	return m_data[index];
}
