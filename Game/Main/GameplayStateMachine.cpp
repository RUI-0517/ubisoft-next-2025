#include "stdafx.h"
#include "GameplayStateMachine.h"

#include "GolfAimingState.h"
#include "ObservationState.h"
#include "App/app.h"

Vector3f GameplayStateMachine::topViewCameraPosition{0.0f, 20.0f, 0.0f};
Vector3f GameplayStateMachine::playerAimCameraOffset{0.0f, 4.0f, -10.0f};

void GameplayStateMachine::on_init()
{
	auto graph = shared_from_this();
	const auto observationState = std::make_shared<ObservationState>(graph);
	const auto golfAimingState = std::make_shared<GolfAimingState>(graph);

	auto observationToGolfAim = [this]() -> bool
	{
		return isObservationKeyPressed;
	};
	observationState->addRelation(golfAimingState, observationToGolfAim);

	auto golfAimToObservation = [this]() -> bool
	{
		return isAimKeyPressed;
	};
	golfAimingState->addRelation(observationState, golfAimToObservation);

	addState(observationState);
	addState(golfAimingState);

	switchState(0);
}

void GameplayStateMachine::on_update(float deltaTimeInSecond)
{
	constexpr int observeKey = 'O';
	if (isObservationKeyPressed && !App::IsKeyPressed(observeKey))
		isObservationKeyPressed = false;
	if (App::IsKeyPressed(observeKey) && !isObservationKeyPressed)
		isObservationKeyPressed = true;

	constexpr int aimKey = 'O';
	if (isAimKeyPressed && !App::IsKeyPressed(aimKey))
		isAimKeyPressed = false;
	if (App::IsKeyPressed(aimKey) && !isAimKeyPressed)
		isAimKeyPressed = true;
}
