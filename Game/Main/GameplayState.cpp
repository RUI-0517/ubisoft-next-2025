#include "stdafx.h"
#include "GameplayState.h"

std::shared_ptr<GameplayStateMachine> GameplayState::get_graph() const
{
	const std::shared_ptr<StateMachineGraph>& graph = getGraph();
	return std::static_pointer_cast<GameplayStateMachine>(graph);
}
