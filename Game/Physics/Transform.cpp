#include "pch.h"
#include "Transform.h"

Transform::Transform(): scale(Vector3f{1, 1, 1})
{
}

Vector3f Transform::TransformPoint(const Vector3f& point) const
{
	return {};
}
