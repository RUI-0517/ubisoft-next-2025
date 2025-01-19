#pragma once
#include "Vector.h"

// N * N Matrix
template <size_t N, typename T>
struct Matrix final
{
	Vector<N, float> m_rows[N];

	static Matrix identity()
	{
		Matrix result{};

		for (size_t i = 0; i < N; ++i)
			result.m_rows[i][i] = 1.0f;

		return result;
	}

	template <typename = std::enable_if_t<N == 4>>
	[[nodiscard]] static Matrix<N, float> translate(float delta)
	{
		return {
			1.0f, 0.0f, 0.0f, delta,
			0.0f, 1.0f, 0.0f, delta,
			0.0f, 0.0f, 1.0f, delta,
			0.0f, 0.0f, 0.0f, 1.0f,
		};
	}

	template <typename = std::enable_if_t<N == 4>>
	[[nodiscard]] static Matrix<N, float> translate(float tx, float ty, float tz)
	{
		return {
			1.0f, 0.0f, 0.0f, tx,
			0.0f, 1.0f, 0.0f, ty,
			0.0f, 0.0f, 1.0f, tz,
			0.0f, 0.0f, 0.0f, 1.0f,
		};
	}

	template <typename = std::enable_if_t<N == 4>>
	[[nodiscard]] static Matrix<N, float> translate(const Vector3f& translation)
	{
		return {
			1.0f, 0.0f, 0.0f, translation.x,
			0.0f, 1.0f, 0.0f, translation.y,
			0.0f, 0.0f, 1.0f, translation.z,
			0.0f, 0.0f, 0.0f, 1.0f,
		};
	}

	template <typename = std::enable_if_t<N == 4>>
	[[nodiscard]] static Matrix<N, float> translate(const Vector4f& translation)
	{
		return {
			1.0f, 0.0f, 0.0f, translation.x,
			0.0f, 1.0f, 0.0f, translation.y,
			0.0f, 0.0f, 1.0f, translation.z,
			0.0f, 0.0f, 0.0f, 1.0f,
		};
	}

	template <typename = std::enable_if_t<N == 4>>
	[[nodiscard]] static Matrix<N, float> translate(const Vector3f&& translation)
	{
		return {
			1.0f, 0.0f, 0.0f, translation.x,
			0.0f, 1.0f, 0.0f, translation.y,
			0.0f, 0.0f, 1.0f, translation.z,
			0.0f, 0.0f, 0.0f, 1.0f,
		};
	}

	template <typename = std::enable_if_t<N == 4>>
	[[nodiscard]] static Matrix<N, float> rotate(const Vector4f& quaternion)
	{
		const float w = quaternion.w;
		const float x = quaternion.x;
		const float y = quaternion.y;
		const float z = quaternion.z;

		const float xx = x * x;
		const float yy = y * y;
		const float zz = z * z;
		const float xy = x * y;
		const float xz = x * z;
		const float yz = y * z;
		const float wx = w * x;
		const float wy = w * y;
		const float wz = w * z;

		return {
			1.0f - 2.0f * (yy + zz), 2.0f * (xy - wz), 2.0f * (xz + wy), 0.0f,
			2.0f * (xy + wz), 1.0f - 2.0f * (xx + zz), 2.0f * (yz - wx), 0.0f,
			2.0f * (xz - wy), 2.0f * (yz + wx), 1.0f - 2.0f * (xx + yy), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	}

	template <typename = std::enable_if_t<N == 4>>
	[[nodiscard]] static Matrix<N, float> rotate(const Vector4f&& quaternion)
	{
		const float w = quaternion.w;
		const float x = quaternion.x;
		const float y = quaternion.y;
		const float z = quaternion.z;

		const float xx = x * x;
		const float yy = y * y;
		const float zz = z * z;
		const float xy = x * y;
		const float xz = x * z;
		const float yz = y * z;
		const float wx = w * x;
		const float wy = w * y;
		const float wz = w * z;

		return {
			1.0f - 2.0f * (yy + zz), 2.0f * (xy - wz), 2.0f * (xz + wy), 0.0f,
			2.0f * (xy + wz), 1.0f - 2.0f * (xx + zz), 2.0f * (yz - wx), 0.0f,
			2.0f * (xz - wy), 2.0f * (yz + wx), 1.0f - 2.0f * (xx + yy), 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		};
	}

	[[nodiscard]] static Matrix<N, float> scale(float scale)
	{
		Matrix<N, float> matrix;

		for (size_t i = 0; i < N - 1; ++i)
			matrix[i][i] = scale;

		matrix[N - 1] = {0.0f, 0.0f, 0.0f, 1.0f};

		return matrix;
	}

	template <typename = std::enable_if_t<N == 4>>
	[[nodiscard]] static Matrix<N, float> scale(float sx, float sy, float sz)
	{
		return {
			sx, 0.0f, 0.0f, 0.0f,
			0.0f, sy, 0.0f, 0.0f,
			0.0f, 0.0f, sz, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
		};
	}

	[[nodiscard]] static Matrix<N, float> scale(const Vector<N - 1, float>& scale)
	{
		Matrix<N, float> matrix;

		for (size_t i = 0; i < N - 1; ++i)
			matrix[i][i] = scale[i];

		matrix[N - 1] = {0.0f, 0.0f, 0.0f, 1.0f};

		return matrix;
	}

	[[nodiscard]] static Matrix<N, float> scale(const Vector<N - 1, float>&& scale)
	{
		Matrix<N, float> matrix;

		for (size_t i = 0; i < N - 1; ++i)
			matrix[i][i] = scale[i];

		matrix[N - 1] = {0.0f, 0.0f, 0.0f, 1.0f};

		return matrix;
	}

	[[nodiscard]] static Matrix<N, float> scale(const Vector<N, float>& scale)
	{
		Matrix<N, float> matrix;

		for (size_t i = 0; i < N - 1; ++i)
			matrix[i][i] = scale[i];

		matrix[N - 1] = {0.0f, 0.0f, 0.0f, 1.0f};

		return matrix;
	}

	Vector<N, float>& operator[](const size_t index)
	{
		assert(index < N && "Row index out of bounds");
		return m_rows[index];
	}

	const Vector<N, float>& operator[](const size_t index) const
	{
		assert(index < N && "Row index out of bounds");
		return m_rows[index];
	}

	friend Vector<N, float> operator*(const Vector<N, float>& vector, const Matrix<N, float>& matrix)
	{
		Vector<N, float> result;
		for (size_t i = 0; i < N; ++i)
			result[i] = vector.dot(matrix[i]);
		return result;
	}

	// Same Size Version
	// TODO: Support N == 3
	template <typename = std::enable_if_t<N == 4>>
	Matrix<4, float> operator*(const Matrix<4, float>& other) const
	{
		Matrix<4, float> result;

		for (size_t i = 0; i < 4; ++i)
		{
			for (size_t j = 0; j < 4; ++j)
			{
				result[i][j] = 0;
				for (size_t k = 0; k < 4; ++k)
					result[i][j] += (*this)[i][k] * other[k][j];
			}
		}

		return result;
	}

	template <typename = std::enable_if_t<N == 4>>
	Matrix<4, float>& operator*=(const Matrix<4, float>& other)
	{
		return *this *= other;
	}

	Matrix()
	{
		for (size_t i = 0; i < N; ++i)
			m_rows[i] = {};
	}

	explicit Matrix(const std::array<Vector<N, float>, N>& rows)
	{
		for (size_t i = 0; i < N; ++i)
			m_rows[i] = rows[i];
	}

	explicit Matrix(const std::array<Vector<N, float>, N>&& rows)
	{
		for (size_t i = 0; i < N; ++i)
			m_rows[i] = rows[i];
	}

	Matrix(std::initializer_list<float> elements)
	{
		if (elements.size() != N * N)
			throw std::invalid_argument("Initializer list size must match N * N");

		auto it = elements.begin();
		for (size_t i = 0; i < N; ++i)
		{
			if constexpr (N == 3)
				m_rows[i] = {*it++, *it++, *it++};
			else if constexpr (N == 4)
				m_rows[i] = {*it++, *it++, *it++, *it++};
		}
	}

	Matrix(const Matrix& other)
	{
		for (size_t i = 0; i < N; ++i)
			m_rows[i] = other[i];
	}

	Matrix(Matrix&& other) noexcept
	{
		for (size_t i = 0; i < N; ++i)
			m_rows[i] = other[i];
	}

	~Matrix() = default;

	Matrix& operator=(const Matrix& other)
	{
		if (this == &other) return *this;
		return *this;
	}

	Matrix& operator=(Matrix&& other) noexcept
	{
		if (this == &other) return *this;
		return *this;
	}
};
