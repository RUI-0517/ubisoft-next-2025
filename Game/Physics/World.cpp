#include "pch.h"
#include "World.h"

World::World()
{
	bodies.emplace_back(std::make_shared<Body>(1.0f));
	bodies[0]->transform.position = {512, 768 - 128, 0};
	bodies.emplace_back(std::make_shared<Body>(1.0f));
	bodies[1]->transform.position = {512, 768 - 64, 0};

	m_boxGeom = std::make_shared<BoxGeometry>();
	m_sphereGeom = std::make_shared<SphereGeometry>(1.0f);

	m_boxGeom->attachBody(bodies[0]);
	m_sphereGeom->attachBody(bodies[1]);
}

void World::simulate(const float timeStep) const
{
	for (const auto& body : bodies)
	{
		// Forward Euler
		Vector3f acceleration = body->calculateAcceleration();
		acceleration += m_gravity;

		Vector3f& velocity = body->getLinearVelocity();
		velocity += acceleration * timeStep;

		Vector3f& position = body->transform.position;
		position += velocity * timeStep;

		body->resetAccumulatedForce();
	}

	// Collision Detection

	// Collision Resolution
}

void World::setGravity(const Vector3f& gravity)
{
	m_gravity = gravity;
}
