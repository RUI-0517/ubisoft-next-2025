#pragma once
#include "GameplayState.h"

class WinState : public GameplayState
{
public:
	explicit WinState(const std::shared_ptr<StateMachineGraph>& graph) : GameplayState(graph)
	{
	}

protected:
	void on_enter() override;
	void on_update(float deltaTimeInSecond) override;
	void on_render() override;
};
