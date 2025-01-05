#pragma once
#include "Transform.h"

class Body
{
	float m_mass;
	float m_massInv;

	Vector3f m_accumulatedForce;

	Vector3f m_linearVelocity;
	Vector3f m_angularVelocity;

	bool m_isKinematic;

public:
	Transform transform;

	explicit Body(float mass);

	void addForce(const Vector3f& force);
	[[nodiscard]] const Vector3f& getAccumulatedForce() const;
	void resetAccumulatedForce();

	[[nodiscard]] Vector3f calculateAcceleration() const;

	[[nodiscard]] const Vector3f& getLinearVelocity() const;
	Vector3f& getLinearVelocity();
	void setLinearVelocity(const Vector3f& velocity);

	[[nodiscard]] float getMass() const;
	[[nodiscard]] float getMassInv() const;

	void setMass(float mass);

	bool isKinematic() const;
	void setKinematic();
	void setDynamic();
};
