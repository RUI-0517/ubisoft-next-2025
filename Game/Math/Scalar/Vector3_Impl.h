#pragma once
#include "Vector_Impl.h"
#include "SyntacticSugar_Impl.h"

template <typename T>
struct Vector<3, T> final : SyntacticSugar<Vector<3, T>, T>
{
	union
	{
		struct
		{
			T x, y, z;
		};

		struct
		{
			T r, g, b;
		};

		T elements[3];
	};

	/// <summary>
	/// Default Constructor
	/// </summary>
	Vector() = default;

	Vector(T x, T y, T z)
	{
		elements[0] = x;
		elements[1] = y;
		elements[2] = z;
	}

	~Vector() = default;
	Vector(const Vector& other) = default;
	Vector& operator=(const Vector& other) = default;

	Vector(Vector&& other) noexcept = default;
	Vector& operator=(Vector&& other) noexcept = default;

	const T& operator[](const size_t index) const
	{
		assert(index >= 0 && index < 3 && "Index out of bounds");
		return elements[index];
	}

	T& operator[](const size_t index)
	{
		assert(index >= 0 && index < 3 && "Index out of bounds");
		return elements[index];
	}

	Vector cross(const Vector& other) const
	{
		return ::cross(*this, other);
	}

	friend std::ostream& operator<<(std::ostream& os, const Vector& vector)
	{
		os << "[" << vector[0] << ' ' << vector[1] << ' ' << vector[2] << "]";
		return os;
	}
};

using Vector3f = Vector<3, float>;
