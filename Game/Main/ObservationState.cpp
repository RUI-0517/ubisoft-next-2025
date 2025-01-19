#include "stdafx.h"
#include "ObservationState.h"
#include "GameplayStateMachine.h"
#include "App/app.h"

void ObservationState::on_init()
{
}

void ObservationState::on_enter()
{
	const auto gameplayGraph = get_graph();
	gameplayGraph->targetCameraPosition = {0.0f, 20.0f, 0.0f};
	gameplayGraph->targetCameraLookAt = {0.0f, 0.0f, 0.0f};
}

void ObservationState::on_update(float deltaTimeInSecond)
{
}

void ObservationState::on_render()
{
	App::Print(32.0f, 32.0f, "Observation");
	App::Print(32.0f, APP_INIT_WINDOW_HEIGHT - 32.0f, "Press 'O' to Aim", 0.0, 0, 0);
}

void ObservationState::on_shutdown()
{
}
