#pragma once
#include "Geometry.h"

class PlaneGeometry final : public Geometry
{
	float m_size;

	PlaneGeometry();

public:
	explicit PlaneGeometry(float size);
};
