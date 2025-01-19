#include "stdafx.h"
#include "ObservationState.h"
#include "GameplayStateMachine.h"
#include "App/app.h"

void ObservationState::on_init()
{
}

void ObservationState::on_enter()
{
	const std::shared_ptr<StateMachineGraph>& graph = getGraph();
	const std::shared_ptr<GameplayStateMachine>& gameplayGraph = std::static_pointer_cast<GameplayStateMachine>(graph);

	gameplayGraph->targetCameraPosition = {0.0f, 20.0f, 0.0f};
	gameplayGraph->targetCameraLookAt = {0.0f, 0.0f, 0.0f};
}

void ObservationState::on_update(float deltaTimeInSecond)
{
}

void ObservationState::on_render()
{
}

void ObservationState::on_shutdown()
{
}
