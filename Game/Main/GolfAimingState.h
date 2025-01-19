#pragma once
#include "GameplayState.h"

class GolfAimingState final : public GameplayState
{
public:
	explicit GolfAimingState(const std::shared_ptr<StateMachineGraph>& graph) : GameplayState(graph)
	{
	}

protected:
	void on_enter() override;
	void on_update(float deltaTimeInSecond) override;
	void on_shutdown() override;
	void on_render() override;
};
