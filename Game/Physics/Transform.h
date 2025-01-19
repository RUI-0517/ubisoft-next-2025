#pragma once

class Transform
{
public:
	Vector3f position;
	Quaternion rotation;
	Vector3f scale;

	Transform();

	[[nodiscard]] Vector4f transformPoint(const Vector4f& point) const;
};
