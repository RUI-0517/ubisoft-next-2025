#include "stdafx.h"
#include "GameplayStateMachine.h"
#include "ObservationState.h"

Vector3f GameplayStateMachine::topViewCameraPosition{0.0f, 20.0f, 0.0f};
Vector3f GameplayStateMachine::playerAimCameraOffset{0.0f, 4.0f, -10.0f};

void GameplayStateMachine::on_init() 
{
	auto graph = shared_from_this();
	const auto observationState = std::make_shared<ObservationState>(graph);
	addState(observationState);

	switchState(0);
}
