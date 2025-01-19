#include "stdafx.h"
#include "HoleProgressState.h"

#include "App/app.h"

void HoleProgressState::on_enter()
{
	setCanExit(false);

	const auto& graph = get_graph();
	const size_t currentFrame = 9 - graph->remainingHoles();

	const auto& sprite = graph->remainingHoleSprite;
	sprite->SetScale(5.0f);
	graph->targetScale = 0.1f;
	graph->remainingHoleSprite->SetFrame(currentFrame);
}

void HoleProgressState::on_update(const float deltaTimeInSecond)
{
	const auto& graph = get_graph();
	const auto& sprite = graph->remainingHoleSprite;
	const auto& currentScale = sprite->GetScale();

	constexpr float lerpSpeed = 1.5f;

	const float newScale = currentScale + lerpSpeed * deltaTimeInSecond * (graph->targetScale - currentScale);
	sprite->SetScale(newScale);

	if (abs(newScale - graph->targetScale) > 1e-2f) setCanExit(true);
}

void HoleProgressState::on_render()
{
	App::Print(32.0f, 32.0f, "Remaining Holes");

	const auto& graph = get_graph();
	graph->remainingHoleSprite->Draw();
}
