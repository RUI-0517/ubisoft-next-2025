#pragma once
#include <xmmintrin.h>

class Vector3p
{
public:
	Vector3p operator+(const Vector3p& other) const;
	Vector3p& operator+=(const Vector3p& other);
	Vector3p operator-(const Vector3p& other) const;
	Vector3p& operator-=(const Vector3p& other);
	Vector3p operator*(float scalar) const;
	Vector3p& operator*=(float scalar);
	Vector3p operator/(float scalar) const;
	Vector3p& operator/=(float scalar);

	bool operator==(const Vector3p& other) const;
	bool operator!=(const Vector3p& other) const;

	[[nodiscard]] float dot(const Vector3p& other) const;
	[[nodiscard]] Vector3p cross(const Vector3p& other) const;
	[[nodiscard]] float magnitude() const;
	[[nodiscard]] float magnitudeSquared() const;
	[[nodiscard]] Vector3p normalize() const;
	[[nodiscard]] float distance(const Vector3p& other) const;
	[[nodiscard]] float distanceSquared(const Vector3p& other) const;
	[[nodiscard]] Vector3p lerp(const Vector3p& other, float t) const;
	[[nodiscard]] Vector3p hadamard(const Vector3p& other) const;
	[[nodiscard]] Vector3p clamp(float minLength, float maxLength) const;

	// float& operator[](size_t index);
	// const float& operator[](size_t index) const;

	friend std::ostream& operator<<(std::ostream& os, const Vector3p& vector);

	Vector3p();
	Vector3p(float x, float y, float z);
	Vector3p(std::initializer_list<float> elements);
	explicit Vector3p(__m128 value);
	~Vector3p();
	Vector3p(const Vector3p& other);
	Vector3p& operator=(const Vector3p& other);
	Vector3p(Vector3p&& other) noexcept;
	Vector3p& operator=(Vector3p&& other) noexcept;

private:
	alignas(16) __m128 m_value;
};
