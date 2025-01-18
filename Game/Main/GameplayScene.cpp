#include "stdafx.h"
#include "GameplayScene.h"

#include <algorithm>

#include "App/app.h"
#include "App/AppSettings.h"

Vector3f GameplayScene::m_gravity{0, -9.81f, 0};

Vector3f GameplayScene::m_topViewCameraPosition{0.0f, 20.0f, 0.0f};
Vector3f GameplayScene::m_playerAimCameraOffset{0.0f, 4.0f, -10.0f};

GameplayScene::GameplayScene(): m_accumulatedTime(0.0f)
{
	initialize_states();
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

	init_players();
	init_holes();

	switch_state(OBSERVATION);
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

	m_onUpdate[m_userState](deltaTimeInSecond);

	update_camera(deltaTimeInSecond);
	m_renderer->update(deltaTimeInSecond);
}

void GameplayScene::Render()
{
	m_renderer->Render();

	// const Vector3f a{static_cast<float>(APP_INIT_WINDOW_WIDTH), static_cast<float>(APP_INIT_WINDOW_HEIGHT), 0.0f};
	// const Vector3f b{0.0f, 0.0f, 50.0f};
	// const Vector3f result = (a + b) / 2.0f;

	// App::Print(result.x - 10, result.y - 10, "+");
	//
	// const std::shared_ptr<Body>& sphereBody = m_physicsWorld->getBodies().back();
	// const Vector3f& velocity = sphereBody->getLinearVelocity();
	// const Vector3f& position = sphereBody->transform.position;

	m_onRender[m_userState]();
}

void GameplayScene::Shutdown()
{
	if (m_userState != OTHER_PLAYER) m_onExit[m_userState]();
}

void GameplayScene::handle_user_input()
{
	constexpr int stateSwitchKey = 'N';

	if (isStateSwitchKeyPressed && !App::IsKeyPressed(stateSwitchKey))
		isStateSwitchKeyPressed = false;

	if (App::IsKeyPressed(stateSwitchKey) && !isStateSwitchKeyPressed)
	{
		m_userState = static_cast<UserState>((m_userState + 1) % INVALID);
		switch_state(m_userState);
		isStateSwitchKeyPressed = true;
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

void GameplayScene::switch_state(const UserState state)
{
	if (state != INVALID) m_onExit[m_userState]();
	m_userState = state;
	m_onEnter[m_userState]();
}

void GameplayScene::initialize_states()
{
	m_onEnter.emplace_back([this] { on_observation_enter(); });
	m_onUpdate.emplace_back([this](const float deltaTimeInSecond) { on_observation_update(deltaTimeInSecond); });
	m_onRender.emplace_back([this] { on_observation_render(); });
	m_onExit.emplace_back([this] { on_observation_exit(); });

	m_onEnter.emplace_back([this] { on_golf_aim_enter(); });
	m_onUpdate.emplace_back([this](const float deltaTimeInSecond) { on_golf_aim_update(deltaTimeInSecond); });
	m_onRender.emplace_back([this] { on_golf_aim_render(); });
	m_onExit.emplace_back([this] { on_golf_aim_exit(); });

	m_onEnter.emplace_back([this] { on_golf_shoot_enter(); });
	m_onUpdate.emplace_back([this](const float deltaTimeInSecond) { on_golf_shoot_update(deltaTimeInSecond); });
	m_onRender.emplace_back([this] { on_golf_shoot_render(); });
	m_onExit.emplace_back([this] { on_golf_shoot_exit(); });

	m_onEnter.emplace_back([this] { on_other_player_enter(); });
	m_onUpdate.emplace_back([this](const float deltaTimeInSecond) { on_other_player_update(deltaTimeInSecond); });
	m_onRender.emplace_back([this] { on_other_player_render(); });
	m_onExit.emplace_back([this] { on_other_player_exit(); });
}

void GameplayScene::init_players()
{
	// Player #1
	{
		const auto player = m_physicsWorld->createBody(1.0f);
		if (!player)
		{
			throw std::runtime_error("Failed to create body");
		}
		player->transform.position = {0.0f, 10.0f, 0.0f};

		const auto playerGeom = m_physicsWorld->createGeometry<SphereGeometry>(1.0f);
		if (!playerGeom)
		{
			throw std::runtime_error("Failed to create SphereGeometry");
		}
		playerGeom->attachBody(player);

		m_renderer->addRenderObject<SphereObject>(playerGeom, RED);

		m_players[0] = playerGeom;
	}

	// Player #2
	{
		const auto player = m_physicsWorld->createBody(1.0f);
		if (!player)
		{
			throw std::runtime_error("Failed to create body1");
		}
		player->transform.position = {2.0f, 20.0f, 0.0f};

		const auto playerGeom = m_physicsWorld->createGeometry<SphereGeometry>(1.0f);
		if (!playerGeom)
		{
			throw std::runtime_error("Failed to create SphereGeometry1");
		}
		playerGeom->attachBody(player);

		m_renderer->addRenderObject<SphereObject>(playerGeom, GREEN);

		m_players[1] = playerGeom;
	}
}

void GameplayScene::init_holes()
{
	const auto hole = m_physicsWorld->createBody(1.0f);
	hole->setKinematic();
	if (!hole) throw std::runtime_error("Failed to create hole");
	hole->transform.position = {-3.0f, 0.5f, 0.0f};

	const auto holeGeom = m_physicsWorld->createGeometry<SphereGeometry>(0.5f);
	if (!holeGeom) throw std::runtime_error("Failed to create SphereGeometry1");
	holeGeom->attachBody(hole);

	m_renderer->addRenderObject<SphereObject>(holeGeom, YELLOW);

	m_holes.emplace_back(holeGeom);
}

void GameplayScene::on_observation_enter()
{
	m_targetCameraPosition = m_topViewCameraPosition;
	m_targetCameraLookAt = Vector3f{0.0f};
}

void GameplayScene::on_observation_update(float deltaTimeInSecond)
{
	constexpr int stateSwitchKey = 'O';

	if (isObservationKeyPressed && !App::IsKeyPressed(stateSwitchKey))
		isObservationKeyPressed = false;

	if (App::IsKeyPressed(stateSwitchKey) && !isObservationKeyPressed)
	{
		switch_state(GOLF_AIMING);
		isObservationKeyPressed = true;
	}

	App::Print(32.0f, 32.0f, "Observation");
}

void GameplayScene::on_observation_render()
{
	App::Print(32.0f, 32.0f, "Observation");
	App::Print(32.0f, APP_INIT_WINDOW_HEIGHT - 32.0f, "Press 'O' to Aim", 0.0, 0, 0);
}

void GameplayScene::on_observation_exit()
{
}

void GameplayScene::on_golf_aim_enter()
{
	// m_targetCameraPosition = {0.0f, 4.0f, -10.0f};
	const Vector3f& playerPosition = m_players[0]->getTransform().position;
	m_targetCameraPosition = playerPosition + Vector3f{0.0f, 4.0f, -10.0f};
	m_targetCameraLookAt = playerPosition;
}

void GameplayScene::on_golf_aim_update(const float deltaTimeInSecond)
{
	float mouseX, mouseY;
	App::GetMousePos(mouseX, mouseY);

	constexpr int stateSwitchKey = 'O';
	constexpr int viewAdjustLeftKey = 'A';
	constexpr int viewAdjustRightKey = 'D';
	constexpr int shootKey = 'J';
	constexpr int chargeKey = VK_SPACE;

	if (isObservationKeyPressed && !App::IsKeyPressed(stateSwitchKey))
		isObservationKeyPressed = false;

	if (App::IsKeyPressed(stateSwitchKey) && !isObservationKeyPressed)
	{
		switch_state(OBSERVATION);
		isObservationKeyPressed = true;
	}

	float currentYaw = atan2(m_targetCameraPosition.x, m_targetCameraPosition.z);

	float deltaYaw = 0.0f;

	if (App::IsKeyPressed(viewAdjustLeftKey))
		deltaYaw = 1.0f * deltaTimeInSecond;
	else if (App::IsKeyPressed(viewAdjustRightKey))
		deltaYaw = -1.0f * deltaTimeInSecond;

	if (currentYaw > 2.0f * PI) currentYaw -= 2.0f * PI;
	if (currentYaw < 0.0f) currentYaw += 2.0f * PI;

	constexpr float radius = 5.0f;
	m_targetCameraPosition.x += radius * (sin(currentYaw) - sin(currentYaw - deltaYaw));
	m_targetCameraPosition.z += radius * (cos(currentYaw) - cos(currentYaw - deltaYaw));

	// App::Print(32.0f, 32.0f, "Observation");

	previousMouseX = mouseX;
	previousMouseY = mouseY;

	if (isChargePressed && !App::IsKeyPressed(chargeKey))
	{
		isChargePressed = false;
	}

	if (App::IsKeyPressed(chargeKey))
	{
		isChargePressed = true;
		m_chargeTime += static_cast<float>(m_chargeTimeSign) * deltaTimeInSecond;

		m_charge = m_chargeTime / m_maxChargeTime;

		if (m_chargeTime <= 0 || m_chargeTime > m_maxChargeTime)
			m_chargeTimeSign *= -1;
	}

	if (shootKeyPressed && !App::IsKeyPressed(shootKey))
		shootKeyPressed = false;

	if (App::IsKeyPressed(shootKey) && !shootKeyPressed)
	{
		switch_state(GOLF_SHOOT);
		shootKeyPressed = true;
	}
}

void GameplayScene::on_golf_aim_render() const
{
	App::Print(32.0f, 32.0f, "Aim");
	App::Print(32.0f, 56.0f, std::to_string(m_charge).c_str());
	App::Print(32.0f, APP_INIT_WINDOW_HEIGHT - 32.0f, "Press 'O' to Observe", 0.0, 0, 0);
}

void GameplayScene::on_golf_aim_exit()
{
	m_chargeTime = 0.0f;
	m_charge = 0.0f;
	m_chargeTimeSign = 1;
}

void GameplayScene::on_golf_shoot_enter()
{
}

void GameplayScene::on_golf_shoot_update(float deltaTimeInSecond)
{
}

void GameplayScene::on_golf_shoot_render()
{
	App::Print(32.0f, 32.0f, "Shoot");
}

void GameplayScene::on_golf_shoot_exit()
{
}

void GameplayScene::on_other_player_enter()
{
	const Vector3f& playerPosition = m_players[1]->getTransform().position;
	m_targetCameraPosition = playerPosition + Vector3f{0.0f, 4.0f, -10.0f};
	m_targetCameraLookAt = playerPosition;
}

void GameplayScene::on_other_player_update(float deltaTimeInSecond)
{
}

void GameplayScene::on_other_player_render()
{
	App::Print(32.0f, 32.0f, "Other Player's View");
}

void GameplayScene::on_other_player_exit()
{
}
