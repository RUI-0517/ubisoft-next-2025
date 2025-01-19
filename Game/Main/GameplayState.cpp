#include "stdafx.h"
#include "GameplayState.h"

std::shared_ptr<GameplayStateMachine> GameplayState::get_graph()
{
	if (m_graph != nullptr) return m_graph;
	const std::shared_ptr<StateMachineGraph>& graph = getGraph();
	m_graph = std::static_pointer_cast<GameplayStateMachine>(graph);
	return m_graph;
}
