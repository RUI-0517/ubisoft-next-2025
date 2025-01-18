#pragma once
#include "BoxGeometry.h"
#include "SphereGeometry.h"

enum MaterialId : uint8_t
{
	PLANE = 0,
	RED,
	GREEN,
	BLUE,
	YELLOW,
	UNDEFINED
};

class RayMarchingObject
{
	MaterialId materialId;

public:
	explicit RayMarchingObject(MaterialId materialId);

	virtual ~RayMarchingObject() = default;

	std::pair<float, MaterialId> evaluate(const Vector3f& point)
	{
		float sdfValue = evaluateImpl(point);
		return {sdfValue, materialId};
	}

	std::shared_ptr<Geometry> getGeometry()
	{
		return getGeometryImpl();
	}

protected:
	virtual float evaluateImpl(const Vector3f& point) =0;
	virtual std::shared_ptr<Geometry> getGeometryImpl() = 0;
};

class SphereObject final : public RayMarchingObject
{
	std::shared_ptr<SphereGeometry> m_geometry;

public:
	SphereObject(const std::shared_ptr<SphereGeometry>& geometry, MaterialId materialId);
	float evaluateImpl(const Vector3f& point) override;
	std::shared_ptr<Geometry> getGeometryImpl() override;
};

class BoxObject final : public RayMarchingObject
{
	std::shared_ptr<BoxGeometry> m_geometry;

public:
	BoxObject(const std::shared_ptr<BoxGeometry>& geometry, MaterialId materialId);
	float evaluateImpl(const Vector3f& point) override;
	std::shared_ptr<Geometry> getGeometryImpl() override;
};
