#pragma once
#include "Body.h"
#include "SphereGeometry.h"

class World
{
	Vector3f m_gravity;
	std::vector<std::shared_ptr<Body>> m_bodies;
	std::vector<std::shared_ptr<Geometry>> m_geometries;

public:
	World();
	~World() = default;
	World(const World&) = delete;
	World& operator=(const World&) = delete;
	World(World&&) = delete;
	World& operator=(World&&) = delete;

	void simulate(float timeStep) const;
	[[nodiscard]] bool handle_collision() const;
	static std::pair<bool, bool> check_collision_spheres(Geometry& self, Geometry& other);

	void setGravity(const Vector3f& gravity);

	const std::shared_ptr<Body> createBody(float mass);

	template <typename T, typename... Args, typename = std::enable_if_t<std::is_base_of_v<Geometry, T>>>
	std::shared_ptr<T> createGeometry(Args&&... args)
	{
		auto geometry = std::make_shared<T>(std::forward<Args>(args)...);
		m_geometries.emplace_back(geometry);
		return geometry;
	}

	[[nodiscard]] const std::vector<std::shared_ptr<Body>>& getBodies() const;
	[[nodiscard]] const std::vector<std::shared_ptr<Geometry>>& getGeometries() const;
};
