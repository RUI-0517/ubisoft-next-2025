#pragma once
#include <functional>

#include "Body.h"
#include "SphereGeometry.h"

class SphereOnlyWorld
{
	struct CollisionInfo;

	Vector3f m_gravity;
	std::vector<std::shared_ptr<Body>> m_bodies;
	std::vector<std::shared_ptr<SphereGeometry>> m_sphereGeometries;

	std::vector<std::shared_ptr<Body>> m_bodyToRemove;
	std::vector<std::shared_ptr<SphereGeometry>> m_sphereGeometryToRemove;

	float m_timeStep;

public:
	using CollisionCallback = std::function<void(const std::shared_ptr<SphereGeometry>& self,
	                                             const std::shared_ptr<SphereGeometry>& other)>;
	std::vector<CollisionCallback> onCollision;

	SphereOnlyWorld() = default;
	~SphereOnlyWorld() = default;
	SphereOnlyWorld(const SphereOnlyWorld&) = delete;
	SphereOnlyWorld& operator=(const SphereOnlyWorld&) = delete;
	SphereOnlyWorld(SphereOnlyWorld&&) = delete;
	SphereOnlyWorld& operator=(SphereOnlyWorld&&) = delete;

	void update();
	void simulate(float timeStep);
	void setGravity(const Vector3f& gravity);

	std::shared_ptr<Body> createBody();
	std::shared_ptr<Body> createBody(float mass);

	template <typename... Args>
	std::shared_ptr<SphereGeometry> createGeometry(Args&&... args)
	{
		auto geometry = std::make_shared<SphereGeometry>(std::forward<Args>(args)...);
		m_sphereGeometries.emplace_back(geometry);
		return geometry;
	}

	void removeObject(const std::shared_ptr<SphereGeometry>& object);

	[[nodiscard]] const std::vector<std::shared_ptr<Body>>& getBodies() const;
	[[nodiscard]] const std::vector<std::shared_ptr<SphereGeometry>>& getGeometries() const;

private:
#pragma region Internal
	void handle_collision() const;
	void solve_penetration_constraint(const CollisionInfo& collision, const std::shared_ptr<Body>& self,
	                                  const std::shared_ptr<Body>& other) const;

	struct CollisionInfo
	{
		Vector3f normal;

		Vector3f ra;
		Vector3f rb;

		float c;

		CollisionInfo(const Vector3f& normal, const Vector3f& ra, const Vector3f& rb, float c)
			: normal(normal), ra(ra), rb(rb), c(c)
		{
		}
	};
#pragma endregion
};
