#include "stdafx.h"
#include "RayMarchingObject.h"

RayMarchingObject::RayMarchingObject(const float materialId): materialId(materialId)
{
}

SphereObject::SphereObject(const std::shared_ptr<SphereGeometry>& geometry, const float materialId)
	: RayMarchingObject(materialId), m_geometry(geometry)
{
}

float SphereObject::evaluateImpl(const Vector3f& point)
{
	const Vector3f currentPosition = m_geometry->getTransform().position;
	const float radius = m_geometry->getRadius();
	return (currentPosition - point).magnitude() - radius;
}

std::shared_ptr<Geometry> SphereObject::getGeometryImpl()
{
	return m_geometry;
}

BoxObject::BoxObject(const std::shared_ptr<BoxGeometry>& geometry, float materialId)
	: RayMarchingObject(materialId)
{
	m_geometry = geometry;
}

float BoxObject::evaluateImpl(const Vector3f& point)
{
	return 0;
}

std::shared_ptr<Geometry> BoxObject::getGeometryImpl()
{
	return m_geometry;
}
