#include "pch.h"
#include "Body.h"

Body::Body(const float mass)
{
	setMass(mass);
	setDynamic();
}

void Body::addForce(const Vector3f& force)
{
	m_accumulatedForce += force;
}

const Vector3f& Body::getAccumulatedForce() const
{
	return m_accumulatedForce;
}

void Body::resetAccumulatedForce()
{
	m_accumulatedForce = {};
}

Vector3f Body::calculateAcceleration() const
{
	return m_accumulatedForce * m_massInv;
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

float Body::getMass() const
{
	return m_mass;
}

float Body::getMassInv() const
{
	return m_massInv;
}

void Body::setMass(const float mass)
{
	if (mass == 0.0) throw std::invalid_argument("Mass cannot be zero.");

	m_mass = mass;
	m_massInv = 1.0f / mass;
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
