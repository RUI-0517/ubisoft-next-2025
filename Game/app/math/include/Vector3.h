#pragma once
#include <initializer_list>

class Vector3
{
public:
	Vector3 operator+(const Vector3& other) const;
	Vector3& operator+=(const Vector3& other);
	Vector3 operator-(const Vector3& other) const;
	Vector3& operator-=(const Vector3& other);
	Vector3 operator*(float scalar) const;
	Vector3& operator*=(float scalar);
	Vector3 operator/(float scalar) const;
	Vector3& operator/=(float scalar);

	bool operator==(const Vector3& other) const;
	bool operator!=(const Vector3& other) const;

	[[nodiscard]] float dot(const Vector3& other) const;
	[[nodiscard]] Vector3 cross(const Vector3& other) const;

	float& operator[](size_t index);
	const float& operator[](size_t index) const;

	friend std::ostream& operator<<(std::ostream& os, const Vector3& vector);

	Vector3();
	Vector3(float x, float y, float z);
	Vector3(std::initializer_list<float> elements);
	~Vector3();
	Vector3(const Vector3& other);
	Vector3& operator=(const Vector3& other);
	Vector3(Vector3&& other) noexcept;
	Vector3& operator=(Vector3&& other) noexcept;

private:
	float m_data[3];
};
