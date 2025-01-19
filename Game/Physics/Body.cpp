#include "pch.h"
#include "Body.h"

Body::Body(const float mass, const float inertiaTensor): m_accumulatedForce(), m_linearVelocity(), m_angularVelocity()
{
	setMass(mass);
	setInertiaTensor(inertiaTensor);
	setDynamic();
}

void Body::addForce(const Vector3f& force)
{
	m_accumulatedForce += force;
}

void Body::addTorque(const Vector3f& torque)
{
	m_accumulatedTorque += torque;
}

void Body::addForceImpulse(const Vector3f& force)
{
	m_accumulatedForce += force * m_massInv;
}

void Body::addTorqueImpulse(const Vector3f& torque)
{
	m_accumulatedTorque += torque * m_inertiaTensorInv;
}

const Vector3f& Body::getAccumulatedForce() const
{
	return m_accumulatedForce;
}

void Body::resetAccumulatedForce()
{
	m_accumulatedForce = {};
}

void Body::resetAccumulatedTorque()
{
	m_accumulatedTorque = {};
}

Vector3f Body::calculateAcceleration() const
{
	return m_accumulatedForce * m_massInv;
}

Vector3f Body::calculateAngularAcceleration() const
{
	return m_accumulatedTorque * m_inertiaTensorInv;
}

const Vector3f& Body::getLinearVelocity() const
{
	return m_linearVelocity;
}

Vector3f& Body::getLinearVelocity()
{
	return m_linearVelocity;
}

void Body::setLinearVelocity(const Vector3f& velocity)
{
	m_linearVelocity = velocity;
}

const Vector3f& Body::getAngularVelocity() const
{
	return m_angularVelocity;
}

Vector3f& Body::getAngularVelocity()
{
	return m_angularVelocity;
}

void Body::setAngularVelocity(const Vector3f& velocity)
{
	m_angularVelocity = velocity;
}

float Body::getMass() const
{
	return m_mass;
}

float Body::getMassInv() const
{
	return m_massInv;
}

float Body::getInertiaTensor() const
{
	return m_inertiaTensor;
}

float Body::getInertiaTensorInv() const
{
	return m_inertiaTensorInv;
}

void Body::setMass(const float mass)
{
	if (mass == 0.0f) throw std::invalid_argument("Mass cannot be zero.");

	m_mass = mass;
	m_massInv = 1.0f / mass;
}

void Body::setInertiaTensor(const float inertiaTensor)
{
	if (inertiaTensor == 0.0f) throw std::invalid_argument("Inertia tensor cannot be zero.");

	m_inertiaTensor = inertiaTensor;
	m_inertiaTensorInv = 1.0f / m_inertiaTensor;
}

void Body::setSphere(const float mass, const float radius)
{
	constexpr float sphereInertiaCoefficient = 2.0f / 5.0f;
	setMass(mass);
	setInertiaTensor(sphereInertiaCoefficient * m_mass * radius * radius);
}

bool Body::isKinematic() const
{
	return m_isKinematic;
}

void Body::setKinematic()
{
	m_isKinematic = true;
}

void Body::setDynamic()
{
	m_isKinematic = false;
}
