#include "pch.h"
#include "SphereGeometry.h"

SphereGeometry::SphereGeometry(const float radius): m_radius(radius)
{
}

Vector3f SphereGeometry::GetSupportPoint(const Vector3f& direction)
{
	const Transform& transform = getBody()->transform;
	return transform.TransformPoint(transform.position + direction * m_radius);
}
