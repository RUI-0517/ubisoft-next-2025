#pragma once
#include <xmmintrin.h>

struct Vector4p
{
	union
	{
		struct
		{
			float x, y, z, w;
		};

		alignas(16) __m128 m_value;
	};

	Vector4p operator+(const Vector4p& other) const;
	Vector4p& operator+=(const Vector4p& other);
	Vector4p operator-(const Vector4p& other) const;
	Vector4p& operator-=(const Vector4p& other);
	Vector4p operator*(float scalar) const;
	Vector4p& operator*=(float scalar);
	Vector4p operator/(float scalar) const;
	Vector4p& operator/=(float scalar);

	bool operator==(const Vector4p& other) const;
	bool operator!=(const Vector4p& other) const;

	[[nodiscard]] float dot(const Vector4p& other) const;
	[[nodiscard]] Vector4p cross(const Vector4p& other) const;
	[[nodiscard]] float magnitude() const;
	[[nodiscard]] float magnitudeSquared() const;
	[[nodiscard]] Vector4p normalize() const;
	[[nodiscard]] float distance(const Vector4p& other) const;
	[[nodiscard]] float distanceSquared(const Vector4p& other) const;
	[[nodiscard]] Vector4p lerp(const Vector4p& other, float t) const;
	[[nodiscard]] Vector4p hadamard(const Vector4p& other) const;
	[[nodiscard]] Vector4p clamp(float minLength, float maxLength) const;

	virtual float& operator[](size_t index);
	virtual const float& operator[](size_t index) const;

	friend std::ostream& operator<<(std::ostream& os, const Vector4p& vector);

	Vector4p();
	Vector4p(float x, float y, float z, float w);
	Vector4p(std::initializer_list<float> elements);
	explicit Vector4p(__m128 value);
	virtual ~Vector4p();
	Vector4p(const Vector4p& other);
	Vector4p& operator=(const Vector4p& other);
	Vector4p(Vector4p&& other) noexcept;
	Vector4p& operator=(Vector4p&& other) noexcept;

protected:
	virtual std::ostream& ToString(std::ostream& os, const Vector4p& vector) const;
};
