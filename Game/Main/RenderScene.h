#pragma once
#include "RayMarchingRenderer.h"
#include "Scene.h"
#include "World.h"

class RenderScene final : public Scene
{
	static constexpr float m_fixedDeltaTime = 0.02f;
	static Vector3f m_gravity;
	std::unique_ptr<World> m_physicsWorld;

	// Accumulated time for the fixed update loop.
	// The fixed update is triggered when this value exceeds m_fixedDeltaTime.
	float m_accumulatedTime;

	std::unique_ptr<RayMarchingRenderer> m_renderer;

public:
#pragma region The Big Fives
	RenderScene();
	~RenderScene() override;
	RenderScene(const RenderScene& scene) = delete;
	RenderScene& operator=(const RenderScene& scene) = delete;
	RenderScene(RenderScene&& scene) noexcept = delete;
	RenderScene& operator=(RenderScene&& scene) noexcept = delete;
#pragma endregion

#pragma region APIs
	void Init() override;
	void Update(float deltaTimeInSecond) override;
	void Render() override;
	void Shutdown() override;
#pragma endregion

private:
#pragma region Internals
	void update_camera(float deltaTimeInSecond) const;
#pragma endregion
};
