#pragma once
#include "Vector_Impl.h"
#include "SyntacticSugar_Impl.h"

template <typename T>
struct Vector<2, T> final : SyntacticSugar<Vector<2, T>, T>
{
	union
	{
		struct
		{
			T x, y;
		};

		struct
		{
			T u, v;
		};

		T elements[2];
	};

	/**
	 * \brief Default Constructor
	*/
	Vector() = default;

	Vector(T x, T y)
	{
		elements[0] = x;
		elements[1] = y;
	}

	~Vector() = default;
	Vector(const Vector& other) = default;
	Vector& operator=(const Vector& other) = default;
	Vector(Vector&& other) noexcept = default;
	Vector& operator=(Vector&& other) noexcept = default;

	const T& operator[](const size_t index) const
	{
		assert(index >= 0 && index < 2 && "Index out of bounds");
		return elements[index];
	}

	T& operator[](const size_t index)
	{
		assert(index >= 0 && index < 2 && "Index out of bounds");
		return elements[index];
	}

	friend std::ostream& operator<<(std::ostream& os, const Vector& vector)
	{
		os << "[" << vector[0] << ' ' << vector[1] << "]";
		return os;
	}
};

using Vector2f = Vector<2, float>;
