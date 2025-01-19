#include "stdafx.h"
#include "App/app.h"
#include "OtherPlayerState.h"
#include "Solver.h"

const Vector3f& OtherPlayerState::getLinearVelocity()
{
	const auto graph = get_graph();
	return graph->players[1]->getBody()->getLinearVelocity();
}

void OtherPlayerState::on_enter()
{
	determine_target_hole();
}

void OtherPlayerState::on_update(float deltaTimeInSecond)
{
	const auto graph = get_graph();
	const Vector3f& playerPosition = graph->players[1]->getTransform().position;
	graph->targetCameraPosition = playerPosition + GameplayStateMachine::playerAimCameraOffset;
	graph->targetCameraLookAt = playerPosition;

	// Delay a bit like its thinking
	if (!graph->otherActed && getTimeEllipse() > 3.0f)
	{
		shoot();
		graph->otherActed = true;
	}
}

void OtherPlayerState::on_render()
{
	App::Print(32.0f, 32.0f, "Other Player's View");
}

void OtherPlayerState::determine_target_hole()
{
	const auto graph = get_graph();

	graph->otherActed = false;

	const auto& currentPosition = graph->players[1]->getTransform().position;
	const auto& holes = graph->holes;

	const auto nodes = std::make_shared<std::vector<Vector3f>>();
	nodes->reserve(holes.size());

	for (const auto& hole : holes)
	{
		const auto& position = hole->getTransform().position;
		nodes->emplace_back(position);
	}

	// TODO: Server-side solver communicating via TCP
	const Solver solver;
	const auto solution = solver.Solve(currentPosition, nodes);
	m_targetPosition = (*nodes)[solution[0]];
}

void OtherPlayerState::shoot()
{
	const auto graph = get_graph();
	const auto& agent = graph->players[1];
	const auto& currentPosition = agent->getTransform().position;

	const Vector3f direction = (m_targetPosition - currentPosition).normalize();
	constexpr float magnitude = 5.0f;
	auto& velocity = agent->getBody()->getLinearVelocity();
	velocity += direction * magnitude;
}
