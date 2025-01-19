#include "stdafx.h"
#include "GameScene.h"
#include "App/AppSettings.h"

Vector3f GameScene::m_gravity{0.0, 0.0f, 0.0f};

GameScene::GameScene(): m_accumulatedTime(0.0f)
{
}

void GameScene::Init()
{
	// Init physics world
	m_world = std::make_unique<SphereOnlyWorld>();
	m_world->setGravity(m_gravity);

	// Init renderer
	constexpr size_t width = APP_INIT_WINDOW_WIDTH;
	constexpr size_t height = APP_INIT_WINDOW_HEIGHT;
	m_renderer = std::make_unique<RayMarchingRenderer>(width, height);
	m_renderer->getCamera().setPosition({0, 4, -10});

	m_stateMachine = std::make_shared<GameplayStateMachine>();
	m_stateMachine->init();
}

void GameScene::Update(const float deltaTimeInSecond)
{
	m_accumulatedTime += deltaTimeInSecond;
	if (m_accumulatedTime >= m_fixedDeltaTime)
	{
		m_world->simulate(m_fixedDeltaTime);
		m_accumulatedTime -= m_fixedDeltaTime;
	}

	m_stateMachine->update(deltaTimeInSecond);
	update_camera(deltaTimeInSecond);

	m_renderer->update(deltaTimeInSecond);
}

void GameScene::Render()
{
	m_renderer->Render();
	m_stateMachine->render();
}

void GameScene::Shutdown()
{
	m_stateMachine->shutdown();
}

void GameScene::update_camera(const float deltaTimeInSecond) const
{
	Camera& camera = m_renderer->getCamera();

	constexpr float speed = 1.5f;

	camera.position = camera.position.lerp(m_stateMachine->targetCameraPosition, speed * deltaTimeInSecond);
	camera.lookAt = camera.lookAt.lerp(m_stateMachine->targetCameraLookAt, speed * deltaTimeInSecond);

	camera.update();
}
