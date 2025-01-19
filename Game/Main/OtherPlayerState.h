#pragma once
#include "GameplayState.h"

class OtherPlayerState final : public GameplayState
{
public:
	explicit OtherPlayerState(const std::shared_ptr<StateMachineGraph>& graph) : GameplayState(graph)
	{
	}

	const Vector3f& getLinearVelocity();

protected:
	void on_enter() override;
	void on_update(float deltaTimeInSecond) override;
	void on_render() override;

private:
	Vector3f m_targetPosition;
	void determine_target_hole();
	void shoot();
};
