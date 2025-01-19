#pragma once
#include "Transform.h"

class Body
{
	float m_mass;
	float m_massInv;

	// Assume Symmetric
	float m_inertiaTensor;
	float m_inertiaTensorInv;

	Vector3f m_accumulatedForce;
	Vector3f m_accumulatedTorque;

	Vector3f m_linearVelocity;
	Vector3f m_angularVelocity;

	bool m_isKinematic;

public:
	Transform transform;

	Body() = default;
	explicit Body(float mass, float inertiaTensor = 1.0f);

	void addForce(const Vector3f& force);
	void addTorque(const Vector3f& torque);
	void addForceImpulse(const Vector3f& force);
	void addTorqueImpulse(const Vector3f& torque);
	[[nodiscard]] const Vector3f& getAccumulatedForce() const;
	void resetAccumulatedForce();
	void resetAccumulatedTorque();

	[[nodiscard]] Vector3f calculateAcceleration() const;
	[[nodiscard]] Vector3f calculateAngularAcceleration() const;

	[[nodiscard]] const Vector3f& getLinearVelocity() const;
	Vector3f& getLinearVelocity();
	void setLinearVelocity(const Vector3f& velocity);

	[[nodiscard]] const Vector3f& getAngularVelocity() const;
	Vector3f& getAngularVelocity();
	void setAngularVelocity(const Vector3f& velocity);

	[[nodiscard]] float getMass() const;
	[[nodiscard]] float getMassInv() const;

	[[nodiscard]] float getInertiaTensor() const;
	[[nodiscard]] float getInertiaTensorInv() const;

	void setMass(float mass);
	void setInertiaTensor(float inertiaTensor);
	void setSphere(float mass, float radius);

	[[nodiscard]] bool isKinematic() const;
	void setKinematic();
	void setDynamic();
};
