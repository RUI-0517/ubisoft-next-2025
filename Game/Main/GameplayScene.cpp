#include "stdafx.h"
#include "GameplayScene.h"

#include "App/app.h"
#include "App/AppSettings.h"

Vector3f GameplayScene::m_gravity{0, -9.81f, 0};

Vector3f GameplayScene::m_topViewCameraPosition{0.0f, 20.0f, 0.0f};
Vector3f GameplayScene::m_playerCameraOffset{0, -9.81f, 0};

GameplayScene::GameplayScene(): m_accumulatedTime(0.0f), m_userState(OVERVIEW)
{
	m_targetCameraPosition = {0.0f, 4.0f, -10.0f};
	m_targetCameraLookAt = Vector3f{0.0f};
}

GameplayScene::~GameplayScene() = default;

void GameplayScene::Init()
{
	// Init physics world
	m_physicsWorld = std::make_unique<World>();
	m_physicsWorld->setGravity(m_gravity);

	// Init renderer
	constexpr size_t width = APP_INIT_WINDOW_WIDTH;
	constexpr size_t height = APP_INIT_WINDOW_HEIGHT;
	m_renderer = std::make_unique<RayMarchingRenderer>(width, height);

	const auto& body = m_physicsWorld->createBody(1.0f);
	if (!body)
	{
		throw std::runtime_error("Failed to create body");
	}
	body->transform.position = {0.0f, 10.0f, 0.0f};

	const auto& sphereGeom = m_physicsWorld->createGeometry<SphereGeometry>(1.0f);
	if (!sphereGeom)
	{
		throw std::runtime_error("Failed to create SphereGeometry");
	}
	sphereGeom->attachBody(body);

	m_renderer->addRenderObject<SphereObject>(sphereGeom, RED);

	const auto& body1 = m_physicsWorld->createBody(1.0f);
	if (!body1)
	{
		throw std::runtime_error("Failed to create body1");
	}
	body1->transform.position = {2.0f, 20.0f, 0.0f};

	const auto& sphereGeom1 = m_physicsWorld->createGeometry<SphereGeometry>(1.0f);
	if (!sphereGeom1)
	{
		throw std::runtime_error("Failed to create SphereGeometry1");
	}
	sphereGeom1->attachBody(body1);

	m_renderer->addRenderObject<SphereObject>(sphereGeom1, GREEN);
}

void GameplayScene::Update(const float deltaTimeInSecond)
{
	handle_user_input();

	m_accumulatedTime += deltaTimeInSecond;
	if (m_accumulatedTime >= m_fixedDeltaTime)
	{
		m_physicsWorld->simulate(m_fixedDeltaTime);
		m_accumulatedTime -= m_fixedDeltaTime;
	}

	update_camera(deltaTimeInSecond);

	m_renderer->update(deltaTimeInSecond);
}

void GameplayScene::Render()
{
	m_renderer->Render();

	const Vector3f a{static_cast<float>(APP_INIT_WINDOW_WIDTH), static_cast<float>(APP_INIT_WINDOW_HEIGHT), 0.0f};
	const Vector3f b{0.0f, 0.0f, 50.0f};
	const Vector3f result = (a + b) / 2.0f;

	// App::Print(result.x - 10, result.y - 10, "+");
	//
	// const std::shared_ptr<Body>& sphereBody = m_physicsWorld->getBodies().back();
	// const Vector3f& velocity = sphereBody->getLinearVelocity();
	// const Vector3f& position = sphereBody->transform.position;
}

void GameplayScene::Shutdown()
{
}

void GameplayScene::handle_user_input()
{
	constexpr int viewSwitchKey = 'N';

	if (isPressingN && !App::IsKeyPressed(viewSwitchKey))
		isPressingN = false;

	if (App::IsKeyPressed(viewSwitchKey) && !isPressingN)
	{
		if (m_userState == OVERVIEW)
		{
			m_targetCameraPosition = {0.0f, 4.0f, -10.0f};
			m_targetCameraLookAt = Vector3f{0.0f};
			m_userState = SHOOT;
		}
		else if (m_userState == SHOOT)
		{
			m_targetCameraPosition = m_topViewCameraPosition;
			m_userState = OVERVIEW;
		}
		isPressingN = true;
	}
}

void GameplayScene::update_camera(const float deltaTimeInSecond) const
{
	Camera& camera = m_renderer->getCamera();

	constexpr float speed = 1.5f;

	camera.position = camera.position.lerp(m_targetCameraPosition, speed * deltaTimeInSecond);
	camera.lookAt = camera.lookAt.lerp(m_targetCameraLookAt, speed * deltaTimeInSecond);

	camera.update();
}
