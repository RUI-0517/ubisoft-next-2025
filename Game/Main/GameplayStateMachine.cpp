#include "stdafx.h"
#include "App/app.h"
#include "GameplayStateMachine.h"
#include "GolfAimingState.h"
#include "GolfShootState.h"
#include "ObservationState.h"
#include "OtherPlayerState.h"

Vector3f GameplayStateMachine::observationCameraPosition{0.0f, 20.0f, 0.0f};
Vector3f GameplayStateMachine::playerAimCameraOffset{0.0f, 4.0f, -10.0f};

void GameplayStateMachine::on_init()
{
	auto graph = shared_from_this();
	const auto observationState = std::make_shared<ObservationState>(graph);
	const auto golfAimingState = std::make_shared<GolfAimingState>(graph);
	const auto golfShootState = std::make_shared<GolfShootState>(graph);
	const auto otherPlayerState = std::make_shared<OtherPlayerState>(graph);

	auto observationToGolfAim = [this]() -> bool
	{
		return m_isObservationKeyPressed;
	};
	observationState->addTransition(golfAimingState, observationToGolfAim);

	auto golfAimToObservation = [this]() -> bool
	{
		return m_isAimKeyPressed;
	};
	golfAimingState->addTransition(observationState, golfAimToObservation);

	auto golfAimToGolfShoot = [this]() -> bool
	{
		return isShootKeyPressed && charge != 0.0f;
	};
	golfAimingState->addTransition(golfShootState, golfAimToGolfShoot);

	auto golfShootToOtherPlayer = [this]() -> bool
	{
		const auto& playerVelocity = players[0]->getBody()->getLinearVelocity();
		return playerVelocity.magnitudeSquared() < 2e-1f;
	};
	golfShootState->addTransition(otherPlayerState, golfShootToOtherPlayer);

	auto otherPlayerToObservation = [this]() -> bool
	{
		const auto& playerVelocity = players[1]->getBody()->getLinearVelocity();
		return otherActed && playerVelocity.magnitudeSquared() < 2e-1f;
	};
	otherPlayerState->addTransition(observationState, otherPlayerToObservation);

	addState(observationState);
	addState(golfAimingState);
	addState(golfShootState);
	addState(otherPlayerState);

	switchState(0);
}

void GameplayStateMachine::on_update(float deltaTimeInSecond)
{
	constexpr int observeKey = 'O';
	if (m_isObservationKeyPressed && !App::IsKeyPressed(observeKey))
		m_isObservationKeyPressed = false;
	if (App::IsKeyPressed(observeKey) && !m_isObservationKeyPressed)
		m_isObservationKeyPressed = true;

	constexpr int aimKey = 'O';
	if (m_isAimKeyPressed && !App::IsKeyPressed(aimKey))
		m_isAimKeyPressed = false;
	if (App::IsKeyPressed(aimKey) && !m_isAimKeyPressed)
		m_isAimKeyPressed = true;

	constexpr int shootKey = 'J';
	if (isShootKeyPressed && !App::IsKeyPressed(shootKey))
		isShootKeyPressed = false;
	if (App::IsKeyPressed(shootKey) && !isShootKeyPressed)
		isShootKeyPressed = true;
}
