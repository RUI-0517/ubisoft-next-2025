#pragma once
#include "GameplayState.h"

class GolfAimingState final : public GameplayState
{
public:
	explicit GolfAimingState(const std::shared_ptr<StateMachineGraph>& graph) : GameplayState(graph)
	{
	}

protected:
	float m_chargeTime = 0.0f;
	float m_maxChargeTime = 3.0f;
	int m_chargeTimeSign = 1;

	bool m_displayChargeHint = false;

	float accumulatedYaw;

	void on_update(float deltaTimeInSecond) override;
	void on_render() override;
	void on_shutdown() override;

private:
	void handle_rotation(float deltaTimeInSecond, const std::shared_ptr<GameplayStateMachine>& graph);
	void handle_charge(float deltaTimeInSecond, std::shared_ptr<GameplayStateMachine> graph);
};
