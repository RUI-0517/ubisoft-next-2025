#pragma once
#include "Common.h"
#include "Vector.h"
#include <array>

// Enable only if N * N
template <size_t N, typename T = float>
using EnableMatrix = std::enable_if_t<(N == 3 || N == 4) && std::is_same_v<T, float>>;

template <size_t N, typename T>
struct Matrix<N, N, T, EnableMatrix<N, T>> final
{
private:
	union
	{
		alignas(16) Vector<N, float> m_rows[N];
	};

public:
	static Matrix identity()
	{
		Matrix result{};

		for (size_t i = 0; i < N; ++i)
			result.m_rows[i][i] = 1.0f;

		return result;
	}

	template <typename = std::enable_if_t<N == 4>>
	[[nodiscard]] static Matrix<N, N, float> translate(float delta)
	{
		return {
			1.0f, 0.0f, 0.0f, delta,
			0.0f, 1.0f, 0.0f, delta,
			0.0f, 0.0f, 1.0f, delta,
			0.0f, 0.0f, 0.0f, 1.0f,
		};
	}

	template <typename = std::enable_if_t<N == 4>>
	[[nodiscard]] static Matrix<N, N, float> translate(float tx, float ty, float tz)
	{
		return {
			1.0f, 0.0f, 0.0f, tx,
			0.0f, 1.0f, 0.0f, ty,
			0.0f, 0.0f, 1.0f, tz,
			0.0f, 0.0f, 0.0f, 1.0f,
		};
	}

	template <typename = std::enable_if_t<N == 4>>
	[[nodiscard]] static Matrix<N, N, float> translate(const Vector3f& translation)
	{
		return {
			1.0f, 0.0f, 0.0f, translation.x,
			0.0f, 1.0f, 0.0f, translation.y,
			0.0f, 0.0f, 1.0f, translation.z,
			0.0f, 0.0f, 0.0f, 1.0f,
		};
	}

	template <typename = std::enable_if_t<N == 4>>
	[[nodiscard]] static Matrix<N, N, float> translate(const Vector4f& translation)
	{
		return {
			1.0f, 0.0f, 0.0f, translation.x,
			0.0f, 1.0f, 0.0f, translation.y,
			0.0f, 0.0f, 1.0f, translation.z,
			0.0f, 0.0f, 0.0f, 1.0f,
		};
	}

	template <typename = std::enable_if_t<N == 4>>
	[[nodiscard]] static Matrix<N, N, float> translate(const Vector3f&& translation)
	{
		return {
			1.0f, 0.0f, 0.0f, translation.x,
			0.0f, 1.0f, 0.0f, translation.y,
			0.0f, 0.0f, 1.0f, translation.z,
			0.0f, 0.0f, 0.0f, 1.0f,
		};
	}
	
	template <typename = std::enable_if_t<N == 4>>
	[[nodiscard]] static Matrix<N, N, float> rotate(const Vector4f& quaternion)
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
	[[nodiscard]] static Matrix<N, N, float> rotate(const Vector4f&& quaternion)
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

	[[nodiscard]] static Matrix<N, N, float> scale(float scale)
	{
		Matrix<N, N, float> matrix;

		for (size_t i = 0; i < N - 1; ++i)
			matrix[i][i] = scale;

		matrix[N - 1] = {0.0f, 0.0f, 0.0f, 1.0f};

		return matrix;
	}

	template <typename = std::enable_if_t<N == 4>>
	[[nodiscard]] static Matrix<N, N, float> scale(float sx, float sy, float sz)
	{
		return {
			sx, 0.0f, 0.0f, 0.0f,
			0.0f, sy, 0.0f, 0.0f,
			0.0f, 0.0f, sz, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f,
		};
	}

	[[nodiscard]] static Matrix<N, N, float> scale(const Vector<N - 1, float>& scale)
	{
		Matrix<N, N, float> matrix;

		for (size_t i = 0; i < N - 1; ++i)
			matrix[i][i] = scale[i];

		matrix[N - 1] = {0.0f, 0.0f, 0.0f, 1.0f};

		return matrix;
	}

	[[nodiscard]] static Matrix<N, N, float> scale(const Vector<N - 1, float>&& scale)
	{
		Matrix<N, N, float> matrix;

		for (size_t i = 0; i < N - 1; ++i)
			matrix[i][i] = scale[i];

		matrix[N - 1] = {0.0f, 0.0f, 0.0f, 1.0f};

		return matrix;
	}

	[[nodiscard]] static Matrix<N, N, float> scale(const Vector<N, float>& scale)
	{
		Matrix<N, N, float> matrix;

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

	friend Vector<N, float> operator*(const Vector<N, float>& vector, const Matrix<N, N, float>& matrix)
	{
		Vector<N, float> result;
		for (size_t i = 0; i < N; ++i)
			result[i] = vector.dot(matrix[i]);
		return result;
	}

	// Same Size Version
	// TODO: Support N == 3
	template <typename = std::enable_if_t<N == 4>>
	Matrix<4, 4, float> operator*(const Matrix<4, 4, float>& other) const
	{
		Matrix<4, 4, float> result;

		const __m128 row3 = _mm_setr_ps(0.0f, 0.0f, 0.0f, 1.0f);

		/// {x, y, z, w}
		/// _mm_unpacklo_ps(a, b)
		/// out: [a.x, b.x, a.y, b.y]
		/// _mm_unpackhi_ps(a, b)
		/// out: [a.z, b.z, a.w, b.w]
		/// 
		/// e.g. _mm_unpacklo_ps(other[0].m_value, other[1].m_value);
		/// other[0] {00, 01, 02, 03}
		/// other[1] {10, 11, 12, 13}
		/// out: {00, 10, 01, 11}
		///		_mm_unpacklo_ps(other[2].m_value, row3);
		/// other[2] {20, 21, 22, 23}
		/// other[3] {30, 31, 32, 33}
		/// out: {20, 30, 21, 31}
		const __m128 lo01 = _mm_unpacklo_ps(other[0].m_value, other[1].m_value);
		const __m128 lo23 = _mm_unpacklo_ps(other[2].m_value, row3);
		const __m128 hi01 = _mm_unpackhi_ps(other[0].m_value, other[1].m_value);
		const __m128 hi23 = _mm_unpackhi_ps(other[2].m_value, row3);

		/// {x, y, z, w}
		/// _mm_movelh_ps(a, b)
		/// out: [a.x, a.y, b.x, b.y]
		/// _mm_movehl_ps(a, b)
		/// out: [b.z, b.w, a.z, a.w]
		/// 
		/// e.g. variable names indicating extracted components
		/// 
		/// e.g. _mm_movelh_ps(lo01, lo23);
		/// lo01 {00, 10, 01, 11}
		/// lo23 {20, 30, 21, 31}
		/// out: {01, 10, 20, 30} which is col1
		const Vector4f col1{_mm_movelh_ps(lo01, lo23)};
		const Vector4f col2{_mm_movehl_ps(lo23, lo01)};
		const Vector4f col3{_mm_movelh_ps(hi01, hi23)};
		const Vector4f col4{_mm_movehl_ps(hi23, hi01)};

		for (size_t i = 0; i < 4; ++i)
		{
			const Vector4f& row = m_rows[i];

			result[i] = {
				row.dot(col1), row.dot(col2),
				row.dot(col3), row.dot(col4)
			};
		}

		return result;
	}

	template <typename = std::enable_if_t<N == 4>>
	Matrix<4, 4, float>& operator*=(const Matrix<4, 4, float>& other)
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
