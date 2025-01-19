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

public:
	GameScene();
	void Init() override;
	void Update(float deltaTimeInSecond) override;
	void Render() override;
	void Shutdown() override;

private:
	void update_camera(float deltaTimeInSecond) const;
};
