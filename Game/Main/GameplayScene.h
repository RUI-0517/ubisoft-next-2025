#pragma once
#include "RayMarchingRenderer.h"
#include "Scene.h"
#include "World.h"
#include <array>

enum UserState : uint8_t
{
	OBSERVATION,
	GOLF_AIMING,
	GOLF_SHOOT,
	OTHER_PLAYER,
	INVALID
};

class GameplayScene final : public Scene
{
	static constexpr float m_fixedDeltaTime = 0.02f;
	static Vector3f m_gravity;
	std::unique_ptr<World> m_physicsWorld;
	float m_accumulatedTime;

	Vector3f m_targetCameraPosition;
	Vector3f m_targetCameraLookAt;

	static Vector3f m_topViewCameraPosition;
	static Vector3f m_playerAimCameraOffset;

	UserState m_userState;
	bool isStateSwitchKeyPressed = false;
	bool isObservationKeyPressed = false;

	bool isChargePressed = false;
	bool shootKeyPressed = false;

	float m_chargeTime = 0.0f;
	float m_maxChargeTime = 3.75f;
	int m_chargeTimeSign = 1;

	float m_charge = 0.0f;

	float previousMouseX = 0.0f;
	float previousMouseY = 0.0f;

	std::array<std::shared_ptr<Geometry>, 2> m_players;
	std::vector<std::shared_ptr<Geometry>> m_holes;

	// Mini State Machine
	std::vector<std::function<void()>> m_onEnter;
	std::vector<std::function<void(float)>> m_onUpdate;
	std::vector<std::function<void()>> m_onRender;
	std::vector<std::function<void()>> m_onExit;

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
	void handle_user_input();
	void update_camera(float deltaTimeInSecond) const;
	void switch_state(UserState state);
	void init_players();
	void init_holes();

	void initialize_states();

	// Overview State
	void on_observation_enter();
	void on_observation_update(float deltaTimeInSecond);
	void on_observation_render();
	void on_observation_exit();

	// Golf Aim State
	void on_golf_aim_enter();
	void on_golf_aim_update(float deltaTimeInSecond);
	void on_golf_aim_render() const;
	void on_golf_aim_exit();

	// Golf Shoot State
	void on_golf_shoot_enter();
	void on_golf_shoot_update(float deltaTimeInSecond);
	void on_golf_shoot_render();
	void on_golf_shoot_exit();

	// Other Player State
	void on_other_player_enter();
	void on_other_player_update(float deltaTimeInSecond);
	void on_other_player_render();
	void on_other_player_exit();
#pragma endregion
};
