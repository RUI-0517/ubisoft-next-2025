#include "stdafx.h"
#include "PhysicsScene.h"
#include "App/AppSettings.h"

Vector3f PhysicsScene::m_gravity{0, -9.81f, 0};

PhysicsScene::PhysicsScene(): m_accumulatedTime(0.0f)
{
}

PhysicsScene::~PhysicsScene() = default;

void PhysicsScene::Init()
{
	// Init physics world
	m_physicsWorld = std::make_unique<World>();
	m_physicsWorld->setGravity(m_gravity);

	// Init renderer
	constexpr size_t width = APP_INIT_WINDOW_WIDTH;
	constexpr size_t height = APP_INIT_WINDOW_HEIGHT;
	m_renderer = std::make_unique<RayMarchingRenderer>(width, height);
}

void PhysicsScene::Update(const float deltaTimeInSecond)
{
	m_accumulatedTime += deltaTimeInSecond;
	if (m_accumulatedTime >= m_fixedDeltaTime)
	{
		m_physicsWorld->simulate(m_fixedDeltaTime);
		m_accumulatedTime -= m_fixedDeltaTime;
	}

	m_renderer->Update(deltaTimeInSecond);
}

void PhysicsScene::Render()
{
	m_renderer->Render();
}

void PhysicsScene::Shutdown()
{
}
