#pragma once

struct CollisionInfo
{
	Vector3f normal;
	float depth;

	CollisionInfo(const Vector3f& normal, const float depth);
};
