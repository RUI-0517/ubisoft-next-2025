#include "stdafx.h"
#include "WinState.h"
#include "App/app.h"

void WinState::on_enter()
{
	const auto graph = get_graph();
	for (const auto& callback : graph->onGameEnd)
		callback();
}

void WinState::on_update(float deltaTimeInSecond)
{
	const auto graph = get_graph();

	const auto& player = graph->players[0];
	const auto& playerPosition = player->getTransform().position;
	graph->targetCameraLookAt = playerPosition;

	const auto targetPosition = playerPosition + GameplayStateMachine::playerAimCameraOffset;
	graph->targetCameraPosition = targetPosition;
}

void WinState::on_render()
{
	// App::Print(32.0f, 32.0f, "You won!");
	get_graph()->playerWinSprite->Draw();
}
