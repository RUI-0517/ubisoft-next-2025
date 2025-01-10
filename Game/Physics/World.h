#pragma once
#include "Body.h"
#include "PlaneGeometry.h"
#include "SphereGeometry.h"

class World
{
	Vector3f m_gravity;

public:
	// TODO: TEST ONLY, Make it private & refactor as Object Pool
	std::vector<std::shared_ptr<Body>> bodies;

	// TODO: TEST ONLY
	std::shared_ptr<PlaneGeometry> planeGeom;
	std::shared_ptr<SphereGeometry> sphereGeom;

	World();
	~World() = default;
	World(const World&) = delete;
	World& operator=(const World&) = delete;
	World(World&&) = delete;
	World& operator=(World&&) = delete;

	void simulate(float timeStep) const;
	void detectCollision() const;

	void setGravity(const Vector3f& gravity);
};
