#include "pch.h"
#include "World.h"
#include "PlaneGeometry.h"

World::World()
{
	m_bodies.emplace_back(std::make_shared<Body>(1.0f));
	m_bodies[0]->transform.position = {0.0f, 0.0f, 0.0f};
	m_bodies[0]->setKinematic();

	const auto& planeGeom = std::make_shared<PlaneGeometry>(20.0f);
	planeGeom->attachBody(m_bodies[0]);
	m_geometries.push_back(planeGeom);
}

bool World::handle_collision() const
{
	bool noCollision = true;
	const size_t size = m_geometries.size();
	for (size_t i = 0; i < size; ++i)
	{
		for (size_t j = i + 1; j < size; ++j)
		{
			auto& self = *m_geometries[i];
			auto& other = *m_geometries[j];

			auto [collide, areSpheres] = check_collision_spheres(self, other);
			if (areSpheres)
			{
				noCollision = !collide;
				continue;
			}

			auto [collided, vertices] = Geometry::checkCollision(self, other);
			if (collided)
			{
				noCollision = false;
				const CollisionInfo info = Geometry::calculateCollisionInfo(std::move(vertices), self, other);

				other.getBody()->transform.position += info.depth * info.normal * 0.3f;
				// sphereBody->setKinematic();
				Vector3f& velocity = other.getBody()->getLinearVelocity();
				velocity.y *= -0.7f;

				if (velocity.magnitudeSquared() <= 0.2f) velocity = Vector3f{0.0f};
			}
		}
	}

	return noCollision;
}

std::pair<bool, bool> World::check_collision_spheres(Geometry& self, Geometry& other)
{
	if (const auto* sphereSelf = dynamic_cast<SphereGeometry*>(&self))
	{
		if (const auto* sphereOther = dynamic_cast<SphereGeometry*>(&other))
		{
			const float selfRadius = sphereSelf->getRadius();
			const float otherRadius = sphereOther->getRadius();

			const Vector3f& selfCenter = sphereSelf->getTransform().position;
			const Vector3f& otherCenter = sphereOther->getTransform().position;

			const float distanceSquared = selfCenter.distanceSquared(otherCenter);
			const float radiusSum = selfRadius + otherRadius;
			return {distanceSquared <= radiusSum * radiusSum, true};
		}
	}
	return {false, false};
}

void World::simulate(const float timeStep) const
{
	for (const auto& body : m_bodies)
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

	// constexpr size_t maxIteration = 5;
	// for (size_t iterationCount = 0; iterationCount < maxIteration; ++iterationCount)
	// {
	// 	const bool noMoreCollision = handle_collision();
	// 	if (noMoreCollision) break;
	// }

	const bool noMoreCollision = handle_collision();

	// Collision Resolution
	//
	// const std::shared_ptr<Body>& sphereBody = m_bodies[1];
	//
	// auto [collided, vertices] = Geometry::checkCollision(*planeGeom, *sphereGeom);
	//
	// if (collided)
	// {
	// 	const CollisionInfo info = Geometry::calculateCollisionInfo(std::move(vertices), *planeGeom, *sphereGeom);
	//
	// 	sphereBody->transform.position += info.depth * info.normal * 0.9f;
	// 	// sphereBody->setKinematic();
	// 	Vector3f& velocity = sphereBody->getLinearVelocity();
	// 	velocity.y *= -0.4f;
	//
	// 	if (velocity.magnitudeSquared() <= 0.2f)
	// 		velocity = Vector3f{0.0f};
	// 	// sphereBody->setLinearVelocity({0.0f, 0.01f, 0.0f});
	// }
}

void World::setGravity(const Vector3f& gravity)
{
	m_gravity = gravity;
}

const std::shared_ptr<Body> World::createBody(float mass)
{
	auto object = std::make_shared<Body>(mass);
	m_bodies.emplace_back(object);
	return object;
}

const std::vector<std::shared_ptr<Body>>& World::getBodies() const
{
	return m_bodies;
}

const std::vector<std::shared_ptr<Geometry>>& World::getGeometries() const
{
	return m_geometries;
}
