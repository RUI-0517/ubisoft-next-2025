#include "pch.h"
#include "World.h"

World::World()
{
	bodies.emplace_back(std::make_shared<Body>(1.0f));
	bodies[0]->transform.position = {0.0f, 0.0f, 0.0f};
	bodies[0]->setKinematic();

	bodies.emplace_back(std::make_shared<Body>(1.0f));
	bodies[1]->transform.position = {0.0f, 10.0f, 0.0f};

	planeGeom = std::make_shared<PlaneGeometry>(2.0f);
	sphereGeom = std::make_shared<SphereGeometry>(1.0f);

	planeGeom->attachBody(bodies[0]);
	sphereGeom->attachBody(bodies[1]);

	geometries.push_back(planeGeom);
	geometries.push_back(sphereGeom);

	transforms.emplace_back(&bodies[1]->transform);
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

	const std::shared_ptr<Body>& sphereBody = bodies[1];

	auto [collided, vertices] = Geometry::checkCollision(*planeGeom, *sphereGeom);

	if (collided)
	{
		const CollisionInfo info = Geometry::calculateCollisionInfo(std::move(vertices), *planeGeom, *sphereGeom);

		sphereBody->transform.position += info.depth * info.normal * 0.9f;
		// sphereBody->setKinematic();
		Vector3f& velocity = sphereBody->getLinearVelocity();
		velocity.y *= -0.4f;

		if (velocity.magnitudeSquared() <= 0.2f)
			velocity = Vector3f{0.0f};
		// sphereBody->setLinearVelocity({0.0f, 0.01f, 0.0f});
	}
}

void World::detectCollision() const
{
}

void World::setGravity(const Vector3f& gravity)
{
	m_gravity = gravity;
}

const std::vector<std::shared_ptr<Body>>& World::getBodies() const
{
	return bodies;
}

const std::vector<std::shared_ptr<Geometry>>& World::getGeometries() const
{
	return geometries;
}

const std::vector<std::shared_ptr<const Transform>>& World::getTransforms() const
{
	return transforms;
}
