#pragma once
#include "RayMarchingRenderer.h"
#include "Scene.h"
#include "SphereOnlyWorld.h"
#include <array>

class PhysicsResolveScene : public Scene
{
	static constexpr float m_fixedDeltaTime = 0.02f;
	float m_accumulatedTime;

	static Vector3f m_gravity;
	std::unique_ptr<SphereOnlyWorld> m_world;
	std::unique_ptr<RayMarchingRenderer> m_renderer;

	std::array<std::shared_ptr<Geometry>, 2> m_players;
	std::vector<std::shared_ptr<Geometry>> m_holes;
	std::vector<std::shared_ptr<SphereGeometry>> m_obstacles;

public:
	PhysicsResolveScene();
	void Init() override;
	void Update(float deltaTimeInSecond) override;
	void Render() override;
	void Shutdown() override;

private:
	void init_players();
	void init_holes();
	void init_obstacles();
};
