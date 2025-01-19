#include "stdafx.h"
#include "GolfShootState.h"
#include "App/app.h"

void GolfShootState::on_enter()
{
	const auto graph = get_graph();
	auto& charge = graph->charge;

	const auto& player = graph->players[0];

	Vector3f forward = graph->targetCameraLookAt - graph->targetCameraPosition;
	forward.y = 0.0f;
	const Vector3f direction = forward.normalize();
	const float magnitude = graph->charge;

	auto& velocity = player->getBody()->getLinearVelocity();
	velocity += direction * magnitude * 20.0f;

	charge = 0.0f;

	++graph->totalStrokes;
}

void GolfShootState::on_update(float deltaTimeInSecond)
{
	const auto gameplayGraph = get_graph();

	const auto& player = gameplayGraph->players[0];
	const auto& playerPosition = player->getTransform().position;
	gameplayGraph->targetCameraLookAt = playerPosition;

	auto targetPosition = GameplayStateMachine::observationCameraPosition;
	targetPosition.x = playerPosition.x;
	targetPosition.z = playerPosition.z - 1.0f;

	gameplayGraph->targetCameraPosition = targetPosition;
}

void GolfShootState::on_render()
{
	App::Print(32.0f, 32.0f, "Shoot");
}
