#pragma once
#include <StateMachineGraph.h>
#include "SphereGeometry.h"
#include <array>

// Use std::enable_shared_from_this<GameplayStateMachine> as a temporary solution for the blackboard implementation
class GameplayStateMachine final : public StateMachineGraph, public std::enable_shared_from_this<GameplayStateMachine>
{
	// TODO: Blackborad
public:
	GameplayStateMachine() = default;

	static Vector3f observationCameraPosition;
	static Vector3f playerAimCameraOffset;

	Vector3f targetCameraPosition;
	Vector3f targetCameraLookAt;

	std::array<std::shared_ptr<Geometry>, 2> players;
	std::vector<std::shared_ptr<Geometry>> holes;
	std::vector<std::shared_ptr<SphereGeometry>> obstacles;

	float charge = 0.0f;
	bool isChargePressed = false;
	bool isShootKeyPressed = false;
	Vector3f playerShootDirection;

	bool otherActed = false;

private:
	bool m_isObservationKeyPressed = false;
	bool m_isAimKeyPressed = false;

	void on_init() override;
	void on_update(float deltaTimeInSecond) override;
};
