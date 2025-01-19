#include "stdafx.h"
#include "App/AppSettings.h"
#include "GameScene.h"
#include <random>

Vector3f GameScene::m_gravity{0.0, -9.81f, 0.0f};

GameScene::GameScene(): m_accumulatedTime(0.0f)
{
}

void GameScene::Init()
{
	// Init physics world
	m_world = std::make_unique<SphereOnlyWorld>();
	m_world->setGravity(m_gravity);

	auto handleCollision = [this](const std::shared_ptr<SphereGeometry>& self,
	                              const std::shared_ptr<SphereGeometry>& other) -> void
	{
		on_collision(self, other);
	};
	m_world->onCollision.emplace_back(handleCollision);

	// Init renderer
	constexpr size_t width = APP_INIT_WINDOW_WIDTH;
	constexpr size_t height = APP_INIT_WINDOW_HEIGHT;
	m_renderer = std::make_unique<RayMarchingRenderer>(width, height);
	m_renderer->getCamera().setPosition({0, 4, -10});

	m_stateMachine = std::make_shared<GameplayStateMachine>();
	init_players();
	init_holes();
	init_obstacles();
	const auto emitParticles = [this]
	{
		emit_particles();
	};
	m_stateMachine->onGameEnd.emplace_back(emitParticles);
	m_stateMachine->init();
}

void GameScene::Update(const float deltaTimeInSecond)
{
	m_world->update();

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

	constexpr float speed = 3.0f;

	camera.position = camera.position.lerp(m_stateMachine->targetCameraPosition, speed * deltaTimeInSecond);
	camera.lookAt = camera.lookAt.lerp(m_stateMachine->targetCameraLookAt, speed * deltaTimeInSecond);

	camera.update();
}

void GameScene::init_players() const
{
	constexpr float radius = 1.0f;

	// Player #1
	{
		const auto player = m_world->createBody();
		player->setSphere(radius, radius);
		if (!player) throw std::runtime_error("Failed to create body");
		player->transform.position = {0.0f, radius, 0.0f};

		const auto playerGeom = m_world->createGeometry(radius);
		playerGeom->tag = "Player";
		if (!playerGeom) throw std::runtime_error("Failed to create SphereGeometry");
		playerGeom->attachBody(player);

		m_renderer->addRenderObject<SphereObject>(playerGeom, RED);
		m_stateMachine->players[0] = playerGeom;
	}

	// Player #2
	{
		const auto player = m_world->createBody();
		player->setSphere(radius, radius);
		if (!player) throw std::runtime_error("Failed to create body");
		player->transform.position = {1.0f, radius, 4.0f};

		const auto playerGeom = m_world->createGeometry(radius);
		playerGeom->tag = "Player";
		if (!playerGeom) throw std::runtime_error("Failed to create SphereGeometry");
		playerGeom->attachBody(player);

		m_renderer->addRenderObject<SphereObject>(playerGeom, BLUE);
		m_stateMachine->players[1] = playerGeom;
	}
}

void GameScene::init_holes() const
{
	constexpr float radius = 0.5f;

	const std::vector<Vector3f> positions{
		{5.0f, radius, 3.0f},
		{-5.0f, radius, 1.0f},
		{3.5f, radius, -4.5f},
		{-2.5f, radius, 7.0f},
		{6.0f, radius, -2.0f}
	};

	for (const auto& position : positions)
	{
		const auto hole = m_world->createBody();
		hole->setSphere(radius, radius);
		hole->setKinematic();
		if (!hole) throw std::runtime_error("Failed to create hole body");
		hole->transform.position = position;

		const auto holeGeom = m_world->createGeometry(radius);
		holeGeom->tag = "Hole";
		if (!holeGeom) throw std::runtime_error("Failed to create hole geometry");
		holeGeom->attachBody(hole);

		m_renderer->addRenderObject<SphereObject>(holeGeom, YELLOW);

		m_stateMachine->holes.emplace_back(holeGeom);
	}
}

void GameScene::init_obstacles() const
{
	constexpr float radius = 0.25f;

	const std::vector<Vector3f> positions{
		{-3.0f, radius, -0.5f},
		{3.0f, radius, -0.5f},
		{1.5f, radius, 5.0f},
		{-1.5f, radius, 5.5f},
		{2.5f, radius, 3.0f},
		{-2.0f, radius, 2.5f},
		{4.0f, radius, 1.0f},
		{-4.0f, radius, 1.0f},
		{0.0f, radius, 6.0f},
		{0.0f, radius, -2.5f},
		{3.5f, radius, 7.0f},
		{-3.5f, radius, 6.5f},
		{5.0f, radius, 0.5f},
		{-5.0f, radius, -1.0f},
		{2.0f, radius, -3.0f},
		{-2.5f, radius, -2.5f}
	};

	for (const auto& position : positions)
	{
		const auto obstacle = m_world->createBody();
		obstacle->setSphere(radius, radius);
		// obstacle->setKinematic();
		if (!obstacle) throw std::runtime_error("Failed to create hole body");
		obstacle->transform.position = position;

		const auto obstacleGeom = m_world->createGeometry(radius);
		obstacleGeom->tag = "Obstacle";
		if (!obstacleGeom) throw std::runtime_error("Failed to create hole geometry");
		obstacleGeom->attachBody(obstacle);

		m_renderer->addRenderObject<SphereObject>(obstacleGeom, GREEN);

		m_stateMachine->obstacles.emplace_back(obstacleGeom);
	}
}

void GameScene::emit_particles() const
{
	const auto& playerPosition = m_stateMachine->players[0]->getTransform().position;

	const Vector3f offset{3.0f, 0.0f, 3.0f};

	Vector3f startPosition = playerPosition - offset;
	startPosition.y = 10.0f;

	Vector3f endPosition = playerPosition + offset;
	endPosition.y = 15.0f;

	constexpr size_t counts = 8;
	constexpr uint8_t materialIndexStart = 1;
	constexpr uint8_t materialIndexEnd = 4;

	constexpr float radius = 0.20f;

	std::random_device device;
	std::mt19937 generator(device());

	std::uniform_real_distribution positionDistX(startPosition.x, endPosition.x);
	std::uniform_real_distribution positionDistY(startPosition.y, endPosition.y);
	std::uniform_real_distribution positionDistZ(startPosition.z, endPosition.z);
	std::uniform_int_distribution<size_t> materialDist(materialIndexStart, materialIndexEnd);

	for (size_t i = 0; i < counts; i++)
	{
		// Create the particle body
		const auto particle = m_world->createBody();
		if (!particle) throw std::runtime_error("Failed to create obstacle");

		particle->setSphere(radius, radius);

		// Random position within startPosition and endPosition
		particle->transform.position.x = positionDistX(generator);
		particle->transform.position.y = positionDistY(generator);
		particle->transform.position.z = positionDistZ(generator);

		// Create the particle geometry
		const auto particleGeom = m_world->createGeometry(radius);
		if (!particleGeom) throw std::runtime_error("Failed to create Particle");

		particleGeom->tag = "Particle";
		particleGeom->attachBody(particle);

		// Random material index between materialIndexStart and materialIndexEnd
		size_t materialIndex = materialDist(generator);
		auto materialId = static_cast<MaterialId>(materialIndex);

		m_renderer->addRenderObject<SphereObject>(particleGeom, materialId);
		m_stateMachine->particles.emplace_back(particleGeom);
	}
}

void GameScene::on_collision(const std::shared_ptr<SphereGeometry>& self,
                             const std::shared_ptr<SphereGeometry>& other) const
{
	const std::string& selfTag = self->tag;
	const std::string& otherTag = other->tag;

	if ((selfTag == "Player" && otherTag == "Hole") || (selfTag == "Hole" && otherTag == "Player"))
	{
		const std::shared_ptr<SphereGeometry> hole = (selfTag == "Hole") ? self : other;

		m_renderer->removeRenderObject(hole);
		m_world->removeObject(hole);
		m_stateMachine->removeHole(hole);
		m_stateMachine->remainingHoleCountChanged = true;
	}
}
