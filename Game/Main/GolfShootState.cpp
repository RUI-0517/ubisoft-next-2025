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
	velocity += direction * magnitude * 10.0f;

	charge = 0.0f;
}

void GolfShootState::on_update(float deltaTimeInSecond)
{
	const auto graph = get_graph();
	const Vector3f& playerPosition = graph->players[0]->getTransform().position;
	graph->targetCameraPosition = playerPosition + Vector3f{0.0f, 12.0f, -20.0f};
	graph->targetCameraLookAt = playerPosition;
}

void GolfShootState::on_render()
{
	App::Print(32.0f, 32.0f, "Shoot");
}
