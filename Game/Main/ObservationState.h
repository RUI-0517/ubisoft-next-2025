#pragma once
#include "State.h"

class ObservationState final : public State
{
public:
	explicit ObservationState(const std::shared_ptr<StateMachineGraph>& graph) : State(graph)
	{
	}

private:
	void on_init() override;
	void on_enter() override;
	void on_update(float deltaTimeInSecond) override;
	void on_render() override;
	void on_shutdown() override;
};
