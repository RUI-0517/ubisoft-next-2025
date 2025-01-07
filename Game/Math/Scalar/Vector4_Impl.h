#pragma once
#include "SyntacticSugar_Impl.h"

template <typename T>
struct Vector<4, T> final : SyntacticSugar<Vector<4, T>, T>
{
	union
	{
		struct
		{
			T x, y, z, w;
		};

		struct
		{
			T r, g, b, a;
		};

		T elements[4];
	};

	/// <summary>
	/// Default Constructor
	/// </summary>
	Vector() = default;

	explicit Vector(const T scalar)
	{
		for (size_t i = 0; i < 4; i++)
			elements[i] = scalar;
	}

	Vector(T x, T y, T z, T w)
	{
		elements[0] = x;
		elements[1] = y;
		elements[2] = z;
		elements[3] = w;
	}

	~Vector() = default;
	Vector(const Vector& other) = default;
	Vector& operator=(const Vector& other) = default;
	Vector(Vector&& other) noexcept = default;
	Vector& operator=(Vector&& other) noexcept = default;

	const T& operator[](const size_t index) const
	{
		assert(index < 4 && "Index out of bounds");
		return elements[index];
	}

	T& operator[](const size_t index)
	{
		assert(index < 4 && "Index out of bounds");
		return elements[index];
	}

	Vector cross(const Vector& other) const
	{
		return ::cross(*this, other);
	}

	friend std::ostream& operator<<(std::ostream& os, const Vector& vector)
	{
		os << "[" << vector[0] << ' ' << vector[1] << ' ' << vector[2] << ' ' << vector[3] << "]";
		return os;
	}
};

using Vector4f = Vector<4, float>;
