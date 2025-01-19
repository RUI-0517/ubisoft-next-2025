#include "stdafx.h"
#include "App/app.h"
#include "App/AppSettings.h"
#include "GolfAimingState.h"


void GolfAimingState::on_update(const float deltaTimeInSecond)
{
	const auto graph = get_graph();

	handle_rotation(deltaTimeInSecond, graph);

	graph->playerShootDirection = graph->targetCameraPosition;

	handle_charge(deltaTimeInSecond, graph);
}

void GolfAimingState::on_render()
{
	const auto graph = get_graph();
	const auto& charge = graph->charge;

	App::Print(32.0f, 32.0f, "Aim");
	const std::string chargePercentage = std::to_string(static_cast<int>(charge * 100)) + "%";
	App::Print(32.0f, 56.0f, chargePercentage.c_str());

	const bool increasing = m_chargeTimeSign > 0;
	const float maxProgressBarWidth = increasing ? 48.0f : 44.0f;
	const char indicator = increasing ? '+' : '-';
	const int progressWidth = static_cast<int>(charge * maxProgressBarWidth);
	const std::string progressBar(progressWidth, indicator);
	App::Print(80.0f, 57.0f, progressBar.c_str());

	App::Print(32.0f, APP_INIT_WINDOW_HEIGHT - 32.0f, "Press 'O' to Observe", 0.0, 0, 0);
	App::Print(32.0f, APP_INIT_WINDOW_HEIGHT - 56.0f, "Press 'A' or 'D' to Rotate", 0.0, 0, 0);
	App::Print(32.0f, APP_INIT_WINDOW_HEIGHT - 80.0f, "Hold 'Space' to Charge", 0.0, 0, 0);
	App::Print(32.0f, APP_INIT_WINDOW_HEIGHT - 104.0f, "Press 'J' to Shoot", 0.0, 0, 0);

	if (graph->isShootKeyPressed && charge == 0.0f) m_displayChargeHint = true;

	if (m_displayChargeHint)
	{
		if (graph->isChargePressed) m_displayChargeHint = false;
		else App::Print(32.0f, APP_INIT_WINDOW_HEIGHT - 128.0f, "Charge required! Hold 'Space' to charge", 1.0, 0, 0);
	}
}

void GolfAimingState::on_shutdown()
{
	m_chargeTime = 0.0f;
	m_chargeTimeSign = 1;
}


void GolfAimingState::handle_rotation(const float deltaTimeInSecond, const std::shared_ptr<GameplayStateMachine>& graph)
{
	const Vector3f& playerPosition = graph->players[0]->getTransform().position;
	auto& targetCameraPosition = graph->targetCameraPosition;

	static Vector3f initialOffset{0.0f, 4.0f, -10.0f};

	constexpr int viewAdjustLeftKey = 'A';
	constexpr int viewAdjustRightKey = 'D';

	float deltaYaw = 0.0f;
	if (App::IsKeyPressed(viewAdjustLeftKey))
	{
		deltaYaw = 1.0f * deltaTimeInSecond;
	}
	else if (App::IsKeyPressed(viewAdjustRightKey))
	{
		deltaYaw = -1.0f * deltaTimeInSecond;
	}

	accumulatedYaw += deltaYaw;

	const float cosYaw = cos(accumulatedYaw);
	const float sinYaw = sin(accumulatedYaw);

	targetCameraPosition = playerPosition + Vector3f{
		cosYaw * initialOffset.x - sinYaw * initialOffset.z,
		initialOffset.y,
		sinYaw * initialOffset.x + cosYaw * initialOffset.z
	};

	graph->targetCameraLookAt = playerPosition;
}

void GolfAimingState::handle_charge(const float deltaTimeInSecond, const std::shared_ptr<GameplayStateMachine> graph)
{
	constexpr int chargeKey = VK_SPACE;
	bool& isChargePressed = graph->isChargePressed;
	if (isChargePressed && !App::IsKeyPressed(chargeKey))
		isChargePressed = false;

	auto& charge = graph->charge;
	if (App::IsKeyPressed(chargeKey))
	{
		isChargePressed = true;
		m_chargeTime += static_cast<float>(m_chargeTimeSign) * deltaTimeInSecond;
		charge = m_chargeTime / m_maxChargeTime;

		if (m_chargeTime <= 0 || m_chargeTime > m_maxChargeTime) m_chargeTimeSign *= -1;
	}
}
