#pragma once
#include "Geometry.h"

class SphereGeometry final : public Geometry
{
	float m_radius;

public:
	explicit SphereGeometry(float radius);
	Vector3f GetSupportPoint(const Vector3f& direction) override;
};
