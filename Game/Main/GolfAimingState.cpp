#include "stdafx.h"
#include "GolfAimingState.h"

#include "App/app.h"
#include "App/AppSettings.h"


void GolfAimingState::on_enter()
{
	// const Vector3f& playerPosition = m_players[0]->getTransform().position;
	// m_targetCameraPosition = playerPosition + Vector3f{0.0f, 4.0f, -10.0f};
	// m_targetCameraLookAt = playerPosition;
}

void GolfAimingState::on_update(const float deltaTimeInSecond)
{
	GameplayState::on_update(deltaTimeInSecond);
}

void GolfAimingState::on_render()
{
	App::Print(32.0f, 32.0f, "Aim");
	// App::Print(32.0f, 56.0f, std::to_string(m_charge).c_str());
	App::Print(32.0f, APP_INIT_WINDOW_HEIGHT - 32.0f, "Press 'O' to Observe", 0.0, 0, 0);
}

void GolfAimingState::on_shutdown()
{
	GameplayState::on_shutdown();
}
