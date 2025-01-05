#pragma once
#include "Body.h"
#include "BoxGeometry.h"
#include "SphereGeometry.h"

class World
{
	Vector3f m_gravity;

	std::shared_ptr<BoxGeometry> m_boxGeom;
	std::shared_ptr<SphereGeometry> m_sphereGeom;

public:
	// TODO: TEST ONLY, Make it private & refactor as Object Pool
	std::vector<std::shared_ptr<Body>> bodies;

	World();
	~World() = default;
	World(const World&) = delete;
	World& operator=(const World&) = delete;
	World(World&&) = delete;
	World& operator=(World&&) = delete;

	void simulate(float timeStep) const;

	void setGravity(const Vector3f& gravity);
};
