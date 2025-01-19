#pragma once
#include <StateMachineGraph.h>
#include "SphereGeometry.h"
#include <array>

// Use std::enable_shared_from_this<GameplayStateMachine> as a temporary solution for the blackboard implementation
class GameplayStateMachine : public StateMachineGraph, public std::enable_shared_from_this<GameplayStateMachine>
{
	// TODO: Blackborad
public:
	Vector3f targetCameraPosition;
	Vector3f targetCameraLookAt;
	static Vector3f topViewCameraPosition;
	static Vector3f playerAimCameraOffset;

	std::array<std::shared_ptr<Geometry>, 2> m_players;
	std::vector<std::shared_ptr<Geometry>> m_holes;
	std::vector<std::shared_ptr<SphereGeometry>> m_obstacles;

	bool isObservationKeyPressed = false;
	bool isAimKeyPressed = false;

	void on_init() override;
	void on_update(float deltaTimeInSecond) override;
};

