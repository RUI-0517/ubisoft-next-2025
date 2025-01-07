#pragma once
#include <ostream>
#include  <cmath>

template <size_t N, typename T>
struct Vector final
{
	T elements[N];

	/// <summary>
	/// Default Constructor
	/// </summary>
	Vector()
	{
		for (size_t i = 0; i < N; i++)
			elements[i] = T();
	}

	explicit Vector(const T scalar)
	{
		for (size_t i = 0; i < N; i++)
			elements[i] = scalar;
	}

	/// <summary>
	/// This allow this kind of init Vector<3, float> vec = {3.14, 6.28, 9.42};
	/// </summary>
	/// <param name="elements"></param>
	template <size_t Size, typename = std::enable_if_t<Size == N>>
	Vector(std::initializer_list<float> elements)
	{
		auto it = elements.begin();
		for (size_t i = 0; i < N; i++)
			this->elements[i] = *it++;
	}

	~Vector() = default;

	Vector(const Vector& other)
	{
		if (this == &other) return;
		std::copy_n(other.elements, N, elements);
	}

	Vector& operator=(const Vector& other)
	{
		if (this != &other)
			std::copy_n(other.elements, N, elements);
		return *this;
	}

	Vector(Vector&& other) noexcept
	{
		std::copy_n(other.elements, N, elements);
	}

	Vector& operator=(Vector&& other) noexcept
	{
		if (this == &other) return *this;
		std::copy_n(other.elements, N, elements);
		return *this;
	}

	const T& operator[](const size_t index) const
	{
		assert(index >= 0 && index < N && "Index out of bounds");
		return (*this)[index];
	}

	T& operator[](const size_t index)
	{
		assert(index > 0 && index < N && "Index out of bounds");
		return (*this)[index];
	}
};

template <size_t N, typename T>
Vector<N, T> operator+(const Vector<N, T>& lhs, const Vector<N, T>& rhs)
{
	Vector<N, T> result;
	for (size_t i = 0; i < N; ++i)
		result[i] = lhs[i] + rhs[i];
	return result;
}

template <size_t N, typename T>
Vector<N, T> operator+(const Vector<N, T>& lhs, const T scalar)
{
	Vector<N, T> result;
	for (size_t i = 0; i < N; ++i)
		result[i] = lhs[i] + scalar;
	return result;
}

template <size_t N, typename T>
Vector<N, T>& operator+=(Vector<N, T>& lhs, const Vector<N, T>& rhs)
{
	return lhs = lhs + rhs;
}

template <size_t N, typename T>
Vector<N, T>& operator+=(Vector<N, T>& lhs, const T scalar)
{
	return lhs = lhs + scalar;
}

// -object
template <size_t N, typename T>
Vector<N, T> operator -(const Vector<N, T>& self)
{
	Vector<N, T> temp;
	for (size_t i = 0; i < N; i++)
		temp[i] = -self[i];
	return temp;
}

template <size_t N, typename T>
Vector<N, T> operator-(const Vector<N, T>& lhs, const Vector<N, T>& rhs)
{
	Vector<N, T> result;
	for (int i = 0; i < N; ++i)
		result[i] = lhs[i] - rhs[i];
	return result;
}

template <size_t N, typename T>
Vector<N, T>& operator-=(Vector<N, T>& lhs, const Vector<N, T>& rhs)
{
	return lhs = lhs - rhs;
}

template <size_t N, typename T>
Vector<N, T> operator*(const Vector<N, T>& vec, const float scalar)
{
	Vector<N, T> result;
	for (int i = 0; i < N; ++i)
		result[i] = vec[i] * scalar;
	return result;
}

template <size_t N, typename T>
Vector<N, T> operator*(const float scalar, const Vector<N, T>& self)
{
	return self * scalar;
}

template <size_t N, typename T>
Vector<N, T>& operator*=(Vector<N, T>& vec, const float scalar)
{
	return vec = vec * scalar;
}

template <size_t N, typename T>
Vector<N, T> operator/(const Vector<N, T>& lhs, const float scalar)
{
	assert(scalar != 0.0f && "Division by zero error in Vector::operator/.");
	Vector<N, T> result;
	for (int i = 0; i < 3; ++i)
	{
		result[i] = lhs[i] * (1 / scalar);
	}
	return result;
}

template <size_t N, typename T>
Vector<N, T>& operator/=(Vector<N, T>& vec, float scalar)
{
	return vec = vec / scalar;
}

template <size_t N, typename T>
bool operator==(const Vector<N, T>& lhs, const Vector<N, T>& rhs)
{
	return std::memcmp(lhs.elements, rhs.elements, N * sizeof(T)) == 0;
}

template <size_t N, typename T>
bool operator!=(const Vector<N, T>& lhs, const Vector<N, T>& rhs)
{
	return !(lhs == rhs);
}

template <size_t N, typename T>
T dot(const Vector<N, T>& lhs, const Vector<N, T>& rhs)
{
	T sum = 0;
	for (size_t i = 0; i < N; i++)
		sum += lhs.elements[i] * rhs.elements[i];
	return sum;
}

template <typename T>
Vector<3, T> cross(const Vector<3, T>& lhs, const Vector<3, T>& rhs)
{
	return {
		lhs[1] * rhs[2] - lhs[2] * rhs[1],
		lhs[2] * rhs[0] - lhs[0] * rhs[2],
		lhs[0] * rhs[1] - lhs[1] * rhs[0]
	};
}

template <typename T>
Vector<4, T> cross(const Vector<4, T>& lhs, const Vector<4, T>& rhs)
{
	return {
		lhs[1] * rhs[2] - lhs[2] * rhs[1],
		lhs[2] * rhs[3] - lhs[3] * rhs[2],
		lhs[3] * rhs[0] - lhs[0] * rhs[3],
		lhs[0] * rhs[1] - lhs[1] * rhs[0]
	};
}

template <size_t N, typename T>
[[nodiscard]] T magnitude(const Vector<N, T>& vec)
{
	return std::sqrt(magnitudeSquared(vec));
}

template <size_t N, typename T>
[[nodiscard]] T magnitudeSquared(const Vector<N, T>& vec)
{
	T sum = 0;
	for (size_t i = 0; i < N; i++)
		sum += vec.elements[i] * vec.elements[i];
	return sum;
}

template <size_t N, typename T>
[[nodiscard]] Vector<N, T> normalize(const Vector<N, T>& vec)
{
	return vec / vec.magnitude();
}

template <size_t N, typename T>
[[nodiscard]] T distance(const Vector<N, T>& lhs, const Vector<N, T>& rhs)
{
	return (lhs - rhs).magnitude();
}

template <size_t N, typename T>
[[nodiscard]] T distanceSquared(const Vector<N, T>& lhs, const Vector<N, T>& rhs)
{
	return (lhs - rhs).magnitudeSquared();
}

template <size_t N, typename T>
[[nodiscard]] Vector<N, T> lerp(const Vector<N, T>& lhs, const Vector<N, T>& rhs, float t)
{
	return lhs * (1 - t) + rhs * t;
}

template <size_t N, typename T>
[[nodiscard]] Vector<N, T> hadamard(const Vector<N, T>& lhs, const Vector<N, T>& rhs)
{
	Vector<N, T> result;

	for (size_t i = 0; i < N; ++i)
		result[i] = lhs[i] * rhs[i];

	return result;
}

template <size_t N, typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
[[nodiscard]] Vector<N, T> pow(const Vector<N, T>& self, const T exponent)
{
	Vector<N, T> result;

	for (size_t i = 0; i < N; ++i)
		result[i] = std::pow(self[i], exponent);

	return result;
}
