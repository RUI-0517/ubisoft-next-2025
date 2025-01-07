#include "pch.h"
#include "Transform.h"

Transform::Transform(): position(), rotation(), scale(Vector3f{1, 1, 1})
{
}

Vector3f Transform::TransformPoint(const Vector3f& point) const
{
	return {};
}
