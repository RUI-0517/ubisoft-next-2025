#pragma once
#include "RayMarchingRenderer.h"
#include "Scene.h"
#include "World.h"

class PhysicsScene final : public Scene
{
	static constexpr float m_fixedDeltaTime = 0.02f;
	static Vector3f m_gravity;
	std::unique_ptr<World> m_physicsWorld;
	float m_accumulatedTime;

	std::unique_ptr<RayMarchingRenderer> m_renderer;

public:
	PhysicsScene();
	~PhysicsScene() override;
	PhysicsScene(const PhysicsScene& scene) = delete;
	PhysicsScene& operator=(const PhysicsScene& scene) = delete;
	PhysicsScene(PhysicsScene&& scene) noexcept = delete;
	PhysicsScene& operator=(PhysicsScene&& scene) noexcept = delete;

	void Init() override;
	void Update(float deltaTimeInSecond) override;
	void Render() override;
	void Shutdown() override;

private:
	// template <typename T, typename T1, typename = std::enable_if_t<std::is_base_of_v<Geometry, T>>, typename... Args>
	// void create_geometry(const std::shared_ptr<Body>& body, MaterialId materialId, Args&&... args)
	// {
	// 	const auto geom = std::make_shared<T>(std::forward<Args>(args)...);
	// 	geom->attachBody(body);
	// 	m_physicsWorld->geometries.push_back(geom);
	// 	m_renderer->addObject<T1>(geom, materialId);
	// }

	void create_geometry(const std::shared_ptr<Body>& body,
	                     const std::shared_ptr<SphereGeometry>& geom, MaterialId materialId) const
	{
		const std::shared_ptr<SphereGeometry>& realGeom = geom;
		realGeom->attachBody(body);
		m_physicsWorld->geometries.push_back(realGeom);
		m_renderer->addObject<SphereObject>(realGeom, materialId);
	}

	void add_shape(const std::shared_ptr<SphereGeometry>& geom);
};
