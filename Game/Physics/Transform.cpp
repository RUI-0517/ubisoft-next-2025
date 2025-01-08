#include "pch.h"
#include "Transform.h"

Transform::Transform(): position(), rotation(), scale(Vector3f{1})
{
}

// TODO: Rotation
Vector3f Transform::transformPoint(const Vector3f& point) const
{
	return point * (Matrix4f::translate(position) * Matrix4f::scale(scale));
}
