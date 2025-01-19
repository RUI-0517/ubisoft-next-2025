#pragma once
#include "GameplayState.h"

class HoleProgressState : public GameplayState
{
public:
	explicit HoleProgressState(const std::shared_ptr<StateMachineGraph>& graph)
		: GameplayState(graph)
	{
	}

protected:
	void on_enter() override;
	void on_update(float deltaTimeInSecond) override;
	void on_render() override;
};
