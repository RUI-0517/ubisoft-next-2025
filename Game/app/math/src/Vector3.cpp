#include <stdafx.h>
#include "../include/Vector3.h"
#include "../include/operations.h"

Vector3& Vector3::operator+=(const Vector3& other)
{
	return *this = *this + other;
}

Vector3& Vector3::operator-=(const Vector3& other)
{
	return *this = *this - other;
}

Vector3& Vector3::operator*=(const float scalar)
{
	return *this = *this * scalar;
}

Vector3& Vector3::operator/=(const float scalar)
{
	return *this = *this / scalar;
}

bool Vector3::operator==(const Vector3& other) const
{
	return distanceSquared(other) == 0.0f;
}

bool Vector3::operator!=(const Vector3& other) const
{
	return !(*this == other);
}

float Vector3::dot(const Vector3& other) const
{
	return m_data[0] * other[0] + m_data[1] * other[1] + m_data[2] * other[2];
}

Vector3 Vector3::cross(const Vector3& other) const
{
	return {
		m_data[1] * other[2] - m_data[2] * other[1],
		m_data[2] * other[0] - m_data[0] * other[2],
		m_data[0] * other[1] - m_data[1] * other[0]
	};
}

float Vector3::magnitude() const
{
	return std::sqrt(m_data[0] * m_data[0] + m_data[1] * m_data[1] + m_data[2] * m_data[2]);
}

float Vector3::magnitudeSquared() const
{
	return m_data[0] * m_data[0] + m_data[1] * m_data[1] + m_data[2] * m_data[2];
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
	return {m_data[0] * other[0], m_data[1] * other[1], m_data[2] * other[2]};
}

Vector3 Vector3::clamp(const float minLength, const float maxLength) const
{
	const float mag = magnitude();
	if (mag < minLength) return normalize() * minLength;
	if (mag > maxLength) return normalize() * maxLength;
	return *this;
}

float& Vector3::operator[](const size_t index)
{
	return m_data[index];
}

const float& Vector3::operator[](const size_t index) const
{
	return m_data[index];
}

std::ostream& operator<<(std::ostream& os, const Vector3& vector)
{
	os << vector.m_data[0] << ' ' << vector.m_data[1] << ' ' << vector.m_data[2];
	return os;
}

Vector3::Vector3()
{
	std::fill(std::begin(m_data), std::end(m_data), 0.0f);
}

Vector3::Vector3(const float x, const float y, const float z): m_data{x, y, z}
{
}

Vector3::Vector3(const float* array)
{
	std::copy_n(array, 3, m_data);
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
	if (this != &other) std::copy_n(other.m_data, 3, m_data);
	return *this;
}
