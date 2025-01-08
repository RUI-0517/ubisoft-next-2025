#pragma once
#include "Geometry.h"

class SphereGeometry final : public Geometry
{
	float m_radius;

public:
	explicit SphereGeometry(float radius);

	[[nodiscard]] Vector3f getSupportPoint(const Vector3f& direction) const override;
};
