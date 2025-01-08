#pragma once

class Transform
{
public:
	Vector3f position;
	// TODO: Refactor as quaternion
	Vector3f rotation;
	Vector3f scale;

	Transform();

	[[nodiscard]] Vector3f transformPoint(const Vector3f& point) const;
};
