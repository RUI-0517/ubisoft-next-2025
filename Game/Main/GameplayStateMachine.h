#pragma once
#include "App/SimpleSprite.h"
#include "SphereGeometry.h"
#include <array>
#include <StateMachineGraph.h>

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

	size_t maxPlayerCount = 2;
	size_t currentPlayerIndex = 0;

	std::array<std::shared_ptr<Geometry>, 2> players;
	std::vector<std::shared_ptr<Geometry>> holes;
	std::vector<std::shared_ptr<SphereGeometry>> obstacles;

	std::vector<std::shared_ptr<SphereGeometry>> particles;

	size_t totalStrokes = 0;

	// Shoot
	float charge = 0.0f;
	bool isChargePressed = false;
	bool isShootKeyPressed = false;
	Vector3f playerShootDirection;

	bool otherActed = false;
	bool remainingHoleCountChanged = false;

	std::vector<std::function<void()>> onGameEnd;

	std::shared_ptr<CSimpleSprite> remainingHoleSprite;
	std::shared_ptr<CSimpleSprite> playerWinSprite;
	float targetScale = 1.0f;

	void removeHole(const std::shared_ptr<SphereGeometry>& hole);
	[[nodiscard]] size_t remainingHoles() const;

	void on_init() override;
	void on_update(float deltaTimeInSecond) override;

private:
	bool m_isObservationKeyPressed = false;
	bool m_isAimKeyPressed = false;
};
