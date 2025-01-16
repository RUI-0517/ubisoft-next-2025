#pragma once
#include <functional>
#include "Transform.h"

class RayMarchingObject
{
	size_t functionIndex;
	float materialId;

	static std::unordered_map<size_t, std::function<float(const Vector3f&)>> sdfFunctions;

public:
	std::pair<float, float> sdf(const Transform& transform, const Vector3f& point)
	{
		const Vector3f newPoint = transform.transformPoint(point);
		float sdfValue = sdfFunctions[functionIndex](newPoint);
		return {sdfValue, materialId};
	}

	static void registerSDF(const size_t index, const std::function<float(const Vector3f&)>& sdfFunc)
	{
		sdfFunctions[index] = sdfFunc;
	}

	[[nodiscard]] static float sd_sphere(const Vector3f& point, float radius);
};
