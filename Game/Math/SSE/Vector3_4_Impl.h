#pragma once
#include <ostream>
#include "Common.h"

template <size_t N, typename T>
struct Vector<N, T, std::enable_if_t<(N == 3 || N == 4) && std::is_same_v<T, float>>> final
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

	Vector operator+(const Vector& other) const
	{
		return Vector(_mm_add_ps(m_value, other.m_value));
	}

	Vector& operator+=(const Vector& other)
	{
		return *this = *this + other;
	}

	Vector operator-(const Vector& other) const
	{
		return Vector(_mm_sub_ps(m_value, other.m_value));
	}

	Vector& operator-=(const Vector& other)
	{
		return *this = *this - other;
	}

	Vector operator*(float scalar) const
	{
		const __m128 packedScalar = _mm_set1_ps(scalar);
		return Vector(_mm_mul_ps(m_value, packedScalar));
	}

	Vector& operator*=(float scalar)
	{
		return *this = *this * scalar;
	}

	Vector operator/(float scalar) const
	{
		const float inverseScalar = 1.0f / scalar;
		const __m128 packedInverseScalar = _mm_set1_ps(inverseScalar);
		return Vector(_mm_mul_ps(m_value, packedInverseScalar));
	}

	Vector& operator/=(float scalar)
	{
		return *this = *this / scalar;
	}

	bool operator==(const Vector& other) const
	{
		const __m128 cmp = _mm_cmpeq_ps(this->m_value, other.m_value);
		const int mask = _mm_movemask_ps(cmp);
		return mask == 0xF;
	}

	bool operator!=(const Vector& other) const
	{
		return !(*this == other);
	}

	[[nodiscard]] float dot(const Vector& other) const
	{
		return dot_impl_scalar(other.m_value);
	}

	// TODO: Optimize
	[[nodiscard]] Vector cross(const Vector& other) const
	{
		// Cross product formula:
		// a[1] * b[2] - a[2] * b[1]
		// a[2] * b[0] - a[0] * b[2]
		// a[0] * b[1] - a[1] * b[0]

		// Shuffle for the first part of the formula:
		// For vector a: {3, 0, 2, 1}, For vector b: {3, 1, 0, 2}
		// The result of multiplying these will give the left side of the cross product.
		// Note: In the _MM_SHUFFLE macro, the index counts from right to left.
		const __m128 tmp0 = _mm_shuffle_ps(m_value, m_value, _MM_SHUFFLE(3, 0, 2, 1));
		const __m128 tmp1 = _mm_shuffle_ps(other.m_value, other.m_value, _MM_SHUFFLE(3, 1, 0, 2));

		// Shuffle for the second part of the formula:
		// This uses the same shuffle logic as before.
		const __m128 tmp2 = _mm_shuffle_ps(m_value, m_value, _MM_SHUFFLE(3, 1, 0, 2));
		const __m128 tmp3 = _mm_shuffle_ps(other.m_value, other.m_value, _MM_SHUFFLE(3, 0, 2, 1));

		// Subtract the two results to complete the cross product calculation.
		return Vector{
			_mm_sub_ps(
				_mm_mul_ps(tmp0, tmp1),
				_mm_mul_ps(tmp2, tmp3)
			)
		};
	}

	[[nodiscard]] float magnitude() const
	{
		return _mm_cvtss_f32(_mm_sqrt_ps(dot_impl(m_value)));
	}

	[[nodiscard]] float magnitudeSquared() const
	{
		return _mm_cvtss_f32(dot_impl(m_value));
	}

	[[nodiscard]] Vector normalize() const
	{
		return Vector(_mm_div_ps(m_value, _mm_sqrt_ps(dot_impl(m_value))));
	}

	[[nodiscard]] float distance(const Vector& other) const
	{
		return (*this - other).magnitude();
	}

	[[nodiscard]] float distanceSquared(const Vector& other) const
	{
		return (*this - other).magnitudeSquared();
	}

	[[nodiscard]] Vector lerp(const Vector& other, float t) const
	{
		return *this * (1 - t) + other * t;
	}

	[[nodiscard]] Vector hadamard(const Vector& other) const
	{
		return Vector(_mm_mul_ps(m_value, other.m_value));
	}

	float& operator[](const size_t index)
	{
		assert(index < N && "Index out of bounds");
		return (&x)[index];
	}

	const float& operator[](const size_t index) const
	{
		assert(index < N && "Index out of bounds");
		return (&x)[index];
	}

	friend std::ostream& operator<<(std::ostream& os, const Vector& vector)
	{
		os << "[";
		if constexpr (N == 3)
			os << vector[0] << ' ' << vector[1] << ' ' << vector[2];
		else if constexpr (N == 4)
			os << vector[0] << ' ' << vector[1] << ' ' << vector[2] << ' ' << vector[3];
		os << "]";
		return os;
	}

	Vector() : m_value(_mm_setzero_ps())
	{
	}

	Vector(const float x, const float y, const float z, const float w = 0.0f)
		: m_value(_mm_setr_ps(x, y, z, w))
	{
	}

	Vector(std::initializer_list<float> elements)
	{
		auto it = elements.begin();
		for (size_t i = 0; i < N; i++)
			(*this)[i] = *it++;
	}

	explicit Vector(const __m128 value) : m_value(value)
	{
	}

	~Vector() = default;
	Vector(const Vector& other) = default;
	Vector& operator=(const Vector& other) = default;
	Vector(Vector&& other) noexcept = default;
	Vector& operator=(Vector&& other) noexcept = default;

#pragma endregion

private:
#pragma region Internal
	/// <summary>
	/// Computes the dot product and replicates it across a __m128 vector.
	/// </summary>
	/// <param name="other">The vector to dot with.</param>
	/// <returns>Dot product in all four components.</returns>
	[[nodiscard]] __m128 dot_impl(__m128 other) const
	{
		// Dot product formula
		// {a.x * b.x + a.y * b.y + a.z * b.z
		// {ab.x + ab.y + ab.z}
		// {ab.xyz}
#if defined(__SSE4_1__) || defined(__AVX__) || defined(__AVX2__)
	// Hardware Implementation
    const __m128 result = _mm_dp_ps(m_value, other, 0xff);
#elif defined(__SSE2__) || defined(_M_IX86) || defined(_M_X64)
		// {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w}
		// {ab.x, ab.y, ab.z, ab.w}
		__m128 temp0 = _mm_mul_ps(m_value, other);
		// Note: In the _MM_SHUFFLE macro, the index counts from right to left.
		// {ab.y, ab.x, ab.w, ab.z}
		__m128 temp1 = _mm_shuffle_ps(temp0, temp0, _MM_SHUFFLE(2, 3, 0, 1));
		// {ab.x + ab.y, ab.y + ab.x, ab.z + ab.w, ab.w + ab.z}
		// {ab.xy, ab.yx, ab.zw, ab.wz}
		// {ab.xy, ab.xy, ab.zw, ab.zw}
		temp0 = _mm_add_ps(temp0, temp1);
		// {ab.zw, ab.zw, ab.xy, ab.xy}
		temp1 = _mm_shuffle_ps(temp0, temp0, _MM_SHUFFLE(0, 1, 2, 3));
		// {ab.xy + ab.zw, ab.xy + ab.zw, ab.zw + ab.xy, ab.zw + ab.xy}
		// {ab.xyzw, ab.xyzw, ab.xyzw, ab.xyzw}
		const __m128 result = _mm_add_ps(temp0, temp1);
#else
    #error Unsupported platform
#endif
		return result;
	}

	/// <summary>
	/// Computes the dot product and returns it as a scalar.
	/// </summary>
	/// <param name="other">The vector to dot with.</param>
	/// <returns>Scalar dot product.</returns>
	[[nodiscard]] float dot_impl_scalar(__m128 other) const
	{
		return _mm_cvtss_f32(dot_impl(other));
	}
#pragma endregion
};
