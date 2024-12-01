#include <stdafx.h>
#include "../include/Vector3.h"

Vector3 Vector3::operator+(const Vector3& other) const
{
	Vector3 result;
	for (int i = 0; i < 3; ++i)
		result[i] = m_data[i] + other[i];
	return result;
}

Vector3& Vector3::operator+=(const Vector3& other)
{
	for (int i = 0; i < 3; ++i)
		m_data[i] += other[i];
	return *this;
}

Vector3 Vector3::operator-(const Vector3& other) const
{
	Vector3 result;
	for (int i = 0; i < 3; ++i)
		result[i] = m_data[i] - other[i];
	return result;
}

Vector3& Vector3::operator-=(const Vector3& other)
{
	for (int i = 0; i < 3; ++i)
		m_data[i] -= other[i];
	return *this;
}

Vector3 Vector3::operator*(const float scalar) const
{
	Vector3 result;
	for (int i = 0; i < 3; ++i)
		result[i] = m_data[i] * scalar;
	return result;
}

Vector3& Vector3::operator*=(const float scalar)
{
	for (float& i : m_data)
		i *= scalar;
	return *this;
}

Vector3 Vector3::operator/(const float scalar) const
{
	if (scalar == 0.0f) throw std::runtime_error("Division by zero error in Vector3::operator/.");

	Vector3 result;
	for (int i = 0; i < 3; ++i)
		result[i] = m_data[i] / scalar;
	return result;
}

Vector3& Vector3::operator/=(const float scalar)
{
	if (scalar == 0.0f) throw std::runtime_error("Division by zero error in Vector3::operator/=.");
	for (float& i : m_data)
		i /= scalar;
	return *this;
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

Vector3 Vector3::lerp(const Vector3& other, float t) const
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
	for (const auto& element : vector.m_data)
		os << element << ' ';
	return os;
}

Vector3::Vector3()
{
	std::fill(std::begin(m_data), std::end(m_data), 0.0f);
}

Vector3::Vector3(const float x, const float y, const float z): m_data{x, y, z}
{
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
