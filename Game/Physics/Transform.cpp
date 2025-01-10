#include "pch.h"
#include "Transform.h"

Transform::Transform(): position(), rotation(Vector4f{0.0f, 0.0f, 0.0f, 1.0f}), scale(Vector3f{1})
{
}

// TODO: Rotation
Vector4f Transform::transformPoint(const Vector4f& point) const
{
	const Matrix4f& transformMatrix =
		Matrix4f::scale(scale) * Matrix4f::rotate(rotation) * Matrix4f::translate(position);
	return Vector4f(point) * transformMatrix;
}
