#include "pch.h"
#include "SphereGeometry.h"

SphereGeometry::SphereGeometry(const float radius): m_radius(radius)
{

}

Vector3f SphereGeometry::getSupportPoint(const Vector3f& direction) const
{
	const Transform& transform = getBody()->transform;
	return transform.transformPoint(transform.position + direction * m_radius);
}
