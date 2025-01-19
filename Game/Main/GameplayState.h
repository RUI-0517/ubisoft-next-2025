#pragma once
#include <State.h>
#include "GameplayStateMachine.h"

class GameplayState : public State
{
public:
	explicit GameplayState(const std::shared_ptr<StateMachineGraph>& graph) : State(graph)
	{
	}

protected:
	[[nodiscard]] std::shared_ptr<GameplayStateMachine> get_graph();

private:
	std::shared_ptr<GameplayStateMachine> m_graph;
};
