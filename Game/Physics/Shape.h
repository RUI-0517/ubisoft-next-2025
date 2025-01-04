#pragma once
#include <vector>
#include "Vector.h"

class Shape
{
	std::vector<Vector3f> m_vertices;

public:
	virtual Vector3f GetSupportPoint(Vector3f direction) = 0;
};
