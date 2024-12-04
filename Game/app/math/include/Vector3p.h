#pragma once
#include "Vector4p.h"

struct Vector3p final : Vector4p
{
	float& operator[](size_t index) override;
	const float& operator[](size_t index) const override;

	Vector3p();
	Vector3p(float _x, float _y, float _z);
	Vector3p(std::initializer_list<float> elements);
	explicit Vector3p(__m128 value);
	Vector3p(const Vector4p& other);
	~Vector3p() override;
	Vector3p(const Vector3p& other);
	Vector3p& operator=(const Vector3p& other);
	Vector3p(Vector3p&& other) noexcept;
	Vector3p& operator=(Vector3p&& other) noexcept;

protected:
	std::ostream& ToString(std::ostream& os, const Vector4p& vector) const override;
};
