#pragma once
#include "Body.h"
#include "SphereGeometry.h"

class SphereOnlyWorld
{
	struct CollisionInfo;

	Vector3f m_gravity;
	std::vector<std::shared_ptr<Body>> m_bodies;
	std::vector<std::shared_ptr<SphereGeometry>> m_sphereGeometries;

	float m_timeStep;

public:
	SphereOnlyWorld() = default;
	~SphereOnlyWorld() = default;
	SphereOnlyWorld(const SphereOnlyWorld&) = delete;
	SphereOnlyWorld& operator=(const SphereOnlyWorld&) = delete;
	SphereOnlyWorld(SphereOnlyWorld&&) = delete;
	SphereOnlyWorld& operator=(SphereOnlyWorld&&) = delete;

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
