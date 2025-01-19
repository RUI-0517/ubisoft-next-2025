#include "stdafx.h"
#include "ObservationState.h"
#include "GameplayStateMachine.h"
#include "App/app.h"

void ObservationState::on_update(const float deltaTimeInSecond)
{
	const auto gameplayGraph = get_graph();

	const auto& player = gameplayGraph->players[0];
	const auto& playerPosition = player->getTransform().position;
	gameplayGraph->targetCameraLookAt = player->getTransform().position;

	auto targetPosition = GameplayStateMachine::observationCameraPosition;
	targetPosition.x = playerPosition.x;
	targetPosition.z = playerPosition.z;

	gameplayGraph->targetCameraPosition = targetPosition;
	gameplayGraph->targetCameraLookAt = player->getTransform().position;
}

void ObservationState::on_render()
{
	App::Print(32.0f, 32.0f, "Observation");
	App::Print(32.0f, APP_INIT_WINDOW_HEIGHT - 32.0f, "Press 'O' to Aim", 0.0, 0, 0);
}

void ObservationState::on_shutdown()
{
}
