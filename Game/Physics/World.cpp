#include "pch.h"
#include "World.h"

World::World()
{
	bodies.emplace_back(std::make_shared<Body>(1.0f));
	bodies[0]->transform.position = {0.0f, 0.0f, 0.0f};
	bodies[0]->setKinematic();

	bodies.emplace_back(std::make_shared<Body>(1.0f));
	bodies[1]->transform.position = {0.0f, 1.0f, 0.0f};

	planeGeom = std::make_shared<PlaneGeometry>(2.0f);
	sphereGeom = std::make_shared<SphereGeometry>(1.0f);

	planeGeom->attachBody(bodies[0]);
	sphereGeom->attachBody(bodies[1]);
}

void World::simulate(const float timeStep) const
{
	for (const auto& body : bodies)
	{
		if (body->isKinematic()) continue;

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
	// detectCollision();

	// Collision Resolution
}

void World::detectCollision() const
{
}

void World::setGravity(const Vector3f& gravity)
{
	m_gravity = gravity;
}
