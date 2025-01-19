#include "stdafx.h"
#include "PhysicsResolveScene.h"

#include "App/AppSettings.h"

Vector3f PhysicsResolveScene::m_gravity{0.0, 0.0f, 0.0f};

PhysicsResolveScene::PhysicsResolveScene(): m_accumulatedTime(0)
{
}

void PhysicsResolveScene::Init()
{
	// Init physics world
	m_world = std::make_unique<SphereOnlyWorld>();
	m_world->setGravity(m_gravity);

	// Init renderer
	constexpr size_t width = APP_INIT_WINDOW_WIDTH;
	constexpr size_t height = APP_INIT_WINDOW_HEIGHT;
	m_renderer = std::make_unique<RayMarchingRenderer>(width, height);
	auto& camera = m_renderer->getCamera();
	camera.position = Vector3f{0.0f, 4.0f, -20.0f};
	camera.lookAt = Vector3f{0, 4, 0};
	camera.update();

	init_players();
	init_holes();
	init_obstacles();

	auto& velocity = m_players[0]->getBody()->getLinearVelocity();
	velocity += {0, 15, 0};

	auto& velocity1 = m_players[1]->getBody()->getLinearVelocity();
	velocity1 += {10, 10, 0};
}

void PhysicsResolveScene::Update(float deltaTimeInSecond)
{
	m_accumulatedTime += deltaTimeInSecond;
	if (m_accumulatedTime >= m_fixedDeltaTime)
	{
		m_world->simulate(m_fixedDeltaTime);
		m_accumulatedTime -= m_fixedDeltaTime;
	}

	m_renderer->update(deltaTimeInSecond);
}

void PhysicsResolveScene::Render()
{
	m_renderer->Render();
}

void PhysicsResolveScene::Shutdown()
{
}

void PhysicsResolveScene::init_players()
{
	// Player #1
	{
		// const auto player = m_physicsWorld->createBody(1.0f);
		const auto player = m_world->createBody(1.0f);
		if (!player)
		{
			throw std::runtime_error("Failed to create body");
		}
		player->transform.position = {0, -5.0f, 0.0f};
		// const auto playerGeom = m_physicsWorld->createGeometry<SphereGeometry>(1.0f);
		const auto playerGeom = m_world->createGeometry(1.0f);
		if (!playerGeom)
		{
			throw std::runtime_error("Failed to create SphereGeometry");
		}
		playerGeom->attachBody(player);
		m_renderer->addRenderObject<SphereObject>(playerGeom, RED);
		m_players[0] = playerGeom;
	}


	// Player #1
	{
		// const auto player = m_physicsWorld->createBody(1.0f);
		const auto player = m_world->createBody(1.0f);
		if (!player)
		{
			throw std::runtime_error("Failed to create body");
		}
		player->transform.position = {0, 1.0f, 0.0f};
		// const auto playerGeom = m_physicsWorld->createGeometry<SphereGeometry>(1.0f);
		const auto playerGeom = m_world->createGeometry(1.0f);
		if (!playerGeom)
		{
			throw std::runtime_error("Failed to create SphereGeometry");
		}
		playerGeom->attachBody(player);
		m_renderer->addRenderObject<SphereObject>(playerGeom, GREEN);
		m_players[1] = playerGeom;
	}
}

void PhysicsResolveScene::init_holes()
{
	{
		const auto hole = m_world->createBody(1.0f);
		hole->setKinematic();
		if (!hole) throw std::runtime_error("Failed to create hole");
		hole->transform.position = {-1.0f, 5.0f, 0.0f};
		const auto holeGeom = m_world->createGeometry(0.5f);
		if (!holeGeom) throw std::runtime_error("Failed to create SphereGeometry1");
		holeGeom->attachBody(hole);
		m_renderer->addRenderObject<SphereObject>(holeGeom, YELLOW);
		m_holes.emplace_back(holeGeom);
	}

	{
		const auto hole = m_world->createBody(1.0f);
		hole->setKinematic();
		if (!hole) throw std::runtime_error("Failed to create hole");
		hole->transform.position = {-2.0f, 5.0f, 0.0f};
		const auto holeGeom = m_world->createGeometry(0.5f);
		if (!holeGeom) throw std::runtime_error("Failed to create SphereGeometry1");
		holeGeom->attachBody(hole);
		m_renderer->addRenderObject<SphereObject>(holeGeom, YELLOW);
		m_holes.emplace_back(holeGeom);
	}

	{
		const auto hole = m_world->createBody(1.0f);
		hole->setKinematic();
		if (!hole) throw std::runtime_error("Failed to create hole");
		hole->transform.position = {1.0f, 5.0f, 0.0f};
		const auto holeGeom = m_world->createGeometry(0.5f);
		if (!holeGeom) throw std::runtime_error("Failed to create SphereGeometry1");
		holeGeom->attachBody(hole);
		m_renderer->addRenderObject<SphereObject>(holeGeom, YELLOW);
		m_holes.emplace_back(holeGeom);
	}

	{
		const auto hole = m_world->createBody(1.0f);
		hole->setKinematic();
		if (!hole) throw std::runtime_error("Failed to create hole");
		hole->transform.position = {2.0f, 5.0f, 0.0f};
		const auto holeGeom = m_world->createGeometry(0.5f);
		if (!holeGeom) throw std::runtime_error("Failed to create SphereGeometry1");
		holeGeom->attachBody(hole);
		m_renderer->addRenderObject<SphereObject>(holeGeom, YELLOW);
		m_holes.emplace_back(holeGeom);
	}
}

void PhysicsResolveScene::init_obstacles()
{
	constexpr float radius = 0.5f;
	// {
	// 	const auto obstacle = m_world->createBody();
	// 	obstacle->setSphere(0.5f, radius);
	// 	if (!obstacle) throw std::runtime_error("Failed to create obstacle");
	// 	obstacle->transform.position = {0.0f, 0.5f, -0.5f};
	// 	const auto obstacleGeom = m_world->createGeometry(radius);
	// 	if (!obstacleGeom) throw std::runtime_error("Failed to create SphereGeometry");
	// 	obstacleGeom->attachBody(obstacle);
	// 	m_renderer->addRenderObject<SphereObject>(obstacleGeom, BLUE);
	// 	m_obstacles.emplace_back(obstacleGeom);
	// }

	constexpr int gridSize = 3;

	for (int y = 1; y <= gridSize; ++y)
	{
		for (int x = -3; x <= 3; ++x)
		{
			const auto obstacle = m_world->createBody();
			if (!obstacle)
				throw std::runtime_error("Failed to create obstacle");

			obstacle->setSphere(0.25f, radius);
			obstacle->transform.position = {static_cast<float>(x), static_cast<float>(y), 0};

			const auto obstacleGeom = m_world->createGeometry(radius);
			if (!obstacleGeom)
				throw std::runtime_error("Failed to create SphereGeometry");

			obstacleGeom->attachBody(obstacle);
			m_renderer->addRenderObject<SphereObject>(obstacleGeom, BLUE);
			m_obstacles.emplace_back(obstacleGeom);
		}
	}
}
