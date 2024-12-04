#pragma once
#include <xmmintrin.h>

#if defined(__SSE4_1__) || defined(__AVX__) || defined(__AVX2__)
#include <smmintrin.h>
#elif defined(__SSE2__) || defined(_M_IX86) || defined(_M_X64)
#include <emmintrin.h>
#else
    #error Unsupported platform
#endif

struct Vector4p
{
#pragma region API

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

#pragma endregion

protected:
#pragma region Internal
	virtual std::ostream& to_string_impl(std::ostream& os, const Vector4p& vector) const;

	/// <summary>
	/// Computes the dot product and replicates it across a __m128 vector.
	/// </summary>
	/// <param name="other">The vector to dot with.</param>
	/// <returns>Dot product in all four components.</returns>
	[[nodiscard]] __m128 dot_impl(__m128 other) const;

	/// <summary>
	/// Computes the dot product and returns it as a scalar.
	/// </summary>
	/// <param name="other">The vector to dot with.</param>
	/// <returns>Scalar dot product.</returns>
	[[nodiscard]] float dot_impl_scalar(__m128 other) const;
#pragma endregion
};
