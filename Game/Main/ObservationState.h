#pragma once
#include "GameplayState.h"

class ObservationState final : public GameplayState
{
public:
	explicit ObservationState(const std::shared_ptr<StateMachineGraph>& graph) : GameplayState(graph)
	{
	}

protected:
	void on_enter() override;
	void on_update(float deltaTimeInSecond) override;
	void on_render() override;
	void on_shutdown() override;
};
