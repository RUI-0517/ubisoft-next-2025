#include "stdafx.h"
#include "RayMarchingObject.h"

float RayMarchingObject::sd_sphere(const Vector3f& point, const float radius)
{
	return point.magnitude() - radius;
}
