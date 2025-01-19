#pragma once
#include <StateMachineGraph.h>
#include "SphereGeometry.h"
#include <array>

class GameplayStateMachine : public StateMachineGraph, public std::enable_shared_from_this<GameplayStateMachine>
{
public:
	Vector3f targetCameraPosition;
	Vector3f targetCameraLookAt;
	static Vector3f topViewCameraPosition;
	static Vector3f playerAimCameraOffset;

	std::array<std::shared_ptr<Geometry>, 2> m_players;
	std::vector<std::shared_ptr<Geometry>> m_holes;
	std::vector<std::shared_ptr<SphereGeometry>> m_obstacles;

	void on_init()  override;
};
