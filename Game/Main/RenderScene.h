#pragma once
#include "Scene.h"
#include "World.h"

class RenderScene final : public Scene
{
	static constexpr float m_fixedDeltaTime = 0.02f;
	static Vector3f m_gravity;
	std::unique_ptr<World> m_physicsWorld;
	float m_accumulatedTime;

public:
	~RenderScene() override;
	void Init() override;
	void Update(float deltaTime) override;
	void Render() override;
	void Shutdown() override;
};
