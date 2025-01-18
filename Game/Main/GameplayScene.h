#pragma once
#include "RayMarchingRenderer.h"
#include "Scene.h"
#include "World.h"

class GameplayScene final : public Scene
{
	static constexpr float m_fixedDeltaTime = 0.02f;
	static Vector3f m_gravity;
	std::unique_ptr<World> m_physicsWorld;
	float m_accumulatedTime;

	std::unique_ptr<RayMarchingRenderer> m_renderer;

public:
	GameplayScene();
	~GameplayScene() override;
	GameplayScene(const GameplayScene& scene) = delete;
	GameplayScene& operator=(const GameplayScene& scene) = delete;
	GameplayScene(GameplayScene&& scene) noexcept = delete;
	GameplayScene& operator=(GameplayScene&& scene) noexcept = delete;

	void Init() override;
	void Update(float deltaTimeInSecond) override;
	void Render() override;
	void Shutdown() override;

private:
#pragma region Internals
	void update_camera(float deltaTimeInSecond) const;
#pragma endregion
};
