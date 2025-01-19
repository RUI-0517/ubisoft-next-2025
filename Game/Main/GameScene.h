#pragma once
#include "GameplayStateMachine.h"
#include "RayMarchingRenderer.h"
#include "Scene.h"
#include "SphereOnlyWorld.h"

class GameScene : public Scene
{
	static constexpr float m_fixedDeltaTime = 0.02f;
	float m_accumulatedTime;

	static Vector3f m_gravity;
	std::unique_ptr<SphereOnlyWorld> m_world;
	std::unique_ptr<RayMarchingRenderer> m_renderer;
	std::shared_ptr<GameplayStateMachine> m_stateMachine;

	size_t m_collisionTime;

public:
	GameScene();
	void Init() override;
	void Update(float deltaTimeInSecond) override;
	void Render() override;
	void Shutdown() override;

private:
#pragma region Internal
	void update_camera(float deltaTimeInSecond) const;
	void init_players() const;
	void init_holes() const;
	void init_obstacles() const;
	void emit_particles()const;

	void on_collision(const std::shared_ptr<SphereGeometry>& self, const std::shared_ptr<SphereGeometry>& other) const;
#pragma endregion
};
