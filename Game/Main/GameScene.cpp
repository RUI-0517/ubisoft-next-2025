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
	init_players();
	init_holes();
	init_obstacles();
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

void GameScene::init_players() const
{
	// Player #1
	{
		// const auto player = m_physicsWorld->createBody(1.0f);
		const auto player = m_world->createBody(1.0f);
		if (!player)
		{
			throw std::runtime_error("Failed to create body");
		}
		player->transform.position = {0.0f, 1.0f, 0.0f};

		// const auto playerGeom = m_physicsWorld->createGeometry<SphereGeometry>(1.0f);
		const auto playerGeom = m_world->createGeometry(1.0f);
		if (!playerGeom)
		{
			throw std::runtime_error("Failed to create SphereGeometry");
		}
		playerGeom->attachBody(player);

		m_renderer->addRenderObject<SphereObject>(playerGeom, RED);

		m_stateMachine->players[0] = playerGeom;
	}

	// Player #2
	{
		const auto player = m_world->createBody(1.0f);
		// const auto player = m_physicsWorld->createBody(1.0f);
		if (!player)
		{
			throw std::runtime_error("Failed to create body1");
		}
		player->transform.position = {2.0f, 1.0f, 0.0f};

		// const auto playerGeom = m_physicsWorld->createGeometry<SphereGeometry>(1.0f);
		const auto playerGeom = m_world->createGeometry(1.0f);
		if (!playerGeom) throw std::runtime_error("Failed to create SphereGeometry");
		playerGeom->attachBody(player);

		m_renderer->addRenderObject<SphereObject>(playerGeom, GREEN);

		m_stateMachine->players[1] = playerGeom;
	}
}

void GameScene::init_holes() const
{
	{
		const auto hole = m_world->createBody(1.0f);
		hole->setKinematic();
		if (!hole) throw std::runtime_error("Failed to create hole");
		hole->transform.position = {-3.0f, 0.5f, 0.0f};

		const auto holeGeom = m_world->createGeometry(0.5f);
		if (!holeGeom) throw std::runtime_error("Failed to create SphereGeometry1");
		holeGeom->attachBody(hole);

		m_renderer->addRenderObject<SphereObject>(holeGeom, YELLOW);

		m_stateMachine->holes.emplace_back(holeGeom);
	}

	{
		const auto hole = m_world->createBody(1.0f);
		hole->setKinematic();
		if (!hole) throw std::runtime_error("Failed to create hole");
		hole->transform.position = {-5.0f, 0.5f, 0.0f};

		const auto holeGeom = m_world->createGeometry(0.5f);
		if (!holeGeom) throw std::runtime_error("Failed to create SphereGeometry1");
		holeGeom->attachBody(hole);

		m_renderer->addRenderObject<SphereObject>(holeGeom, YELLOW);

		m_stateMachine->holes.emplace_back(holeGeom);
	}

	{
		const auto hole = m_world->createBody(1.0f);
		hole->setKinematic();
		if (!hole) throw std::runtime_error("Failed to create hole");
		hole->transform.position = {-5.0f, 0.5f, 2.0f};

		const auto holeGeom = m_world->createGeometry(0.5f);
		if (!holeGeom) throw std::runtime_error("Failed to create SphereGeometry1");
		holeGeom->attachBody(hole);

		m_renderer->addRenderObject<SphereObject>(holeGeom, YELLOW);

		m_stateMachine->holes.emplace_back(holeGeom);
	}
}

void GameScene::init_obstacles() const
{
	constexpr float radius = 0.5f;

	{
		const auto obstacle = m_world->createBody();
		obstacle->setSphere(0.5f, radius);

		if (!obstacle) throw std::runtime_error("Failed to create obstacle");
		obstacle->transform.position = {-3.0f, 0.5f, -0.5f};

		const auto obstacleGeom = m_world->createGeometry(radius);
		if (!obstacleGeom) throw std::runtime_error("Failed to create SphereGeometry");
		obstacleGeom->attachBody(obstacle);

		m_renderer->addRenderObject<SphereObject>(obstacleGeom, BLUE);
		m_stateMachine->obstacles.emplace_back(obstacleGeom);
	}

	{
		const auto obstacle = m_world->createBody();
		obstacle->setSphere(0.5f, radius);

		if (!obstacle) throw std::runtime_error("Failed to create obstacle");
		obstacle->transform.position = {-3.0f, 0.5f, -1.75f};

		const auto obstacleGeom = m_world->createGeometry(radius);
		if (!obstacleGeom) throw std::runtime_error("Failed to create SphereGeometry");
		obstacleGeom->attachBody(obstacle);

		m_renderer->addRenderObject<SphereObject>(obstacleGeom, BLUE);
		m_stateMachine->obstacles.emplace_back(obstacleGeom);
	}
}
