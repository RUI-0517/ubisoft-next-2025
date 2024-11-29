#include "stdafx.h"
#include "../include/Vector3.h"

Vector3 Vector3::operator+(const Vector3& other) const
{
	return {
		m_data[0] + other[0], m_data[1] + other[1], m_data[2] + other[2]
	};
}

Vector3& Vector3::operator+=(const Vector3& other)
{
	m_data[0] += other[0];
	m_data[1] += other[1];
	m_data[2] += other[2];
	return *this;
}

Vector3 Vector3::operator-(const Vector3& other) const
{
	return {
		m_data[0] - other[0], m_data[1] - other[1], m_data[2] - other[2]
	};
}

Vector3& Vector3::operator-=(const Vector3& other)
{
	m_data[0] -= other[0];
	m_data[1] -= other[1];
	m_data[2] -= other[2];
	return *this;
}

Vector3 Vector3::operator*(float scalar) const
{
	return {m_data[0] * scalar, m_data[1] * scalar, m_data[2] * scalar};
}

Vector3& Vector3::operator*=(const float scalar)
{
	m_data[0] *= scalar;
	m_data[1] *= scalar;
	m_data[2] *= scalar;
	return *this;
}

Vector3 Vector3::operator/(const float scalar) const
{
	if (scalar == 0.0f) throw std::runtime_error("Division by zero error in Vector3::operator/.");
	return {m_data[0] / scalar, m_data[1] / scalar, m_data[2] / scalar};
}

Vector3& Vector3::operator/=(const float scalar)
{
	if (scalar == 0.0f) throw std::runtime_error("Division by zero error in Vector3::operator/=.");
	m_data[0] /= scalar;
	m_data[1] /= scalar;
	m_data[2] /= scalar;
	return *this;
}

bool Vector3::operator==(const Vector3& other) const
{
	return std::fabs(m_data[0] - other[0]) < 1e-5
		&& std::fabs(m_data[1] - other[1]) < 1e-5
		&& std::fabs(m_data[2] - other[2]) < 1e-5;
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
	std::fill(std::begin(m_data), std::end(m_data), 0.0f);
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
