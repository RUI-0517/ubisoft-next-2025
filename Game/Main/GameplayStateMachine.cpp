#include "stdafx.h"
#include "App/app.h"
#include "GameplayStateMachine.h"
#include "GolfAimingState.h"
#include "GolfShootState.h"
#include "HoleProgressState.h"
#include "ObservationState.h"
#include "OtherPlayerState.h"
#include "Resources.h"
#include "WinState.h"

Vector3f GameplayStateMachine::observationCameraPosition{0.0f, 20.0f, -4.0f};
Vector3f GameplayStateMachine::playerAimCameraOffset{0.0f, 4.0f, -10.0f};

void GameplayStateMachine::removeHole(const std::shared_ptr<SphereGeometry>& hole)
{
	const auto it = std::find(holes.begin(), holes.end(), hole);
	if (it != holes.end()) holes.erase(it);
}

size_t GameplayStateMachine::remainingHoles() const
{
	return holes.size();
}

void GameplayStateMachine::on_init()
{
	{
		CSimpleSprite* sprite = App::CreateSprite(RESOURCE_PATH("Numbers Inverse.png"), 1, 10);
		sprite->SetFrame(0);
		constexpr int halfWidth = APP_INIT_WINDOW_WIDTH / 2;
		constexpr int halfHeight = APP_INIT_WINDOW_HEIGHT / 2;
		sprite->SetPosition(halfWidth, halfHeight);
		remainingHoleSprite = std::shared_ptr<CSimpleSprite>(sprite);
	}

	{
		CSimpleSprite* sprite = App::CreateSprite(RESOURCE_PATH("Win Sprite.png"), 1, 1);
		constexpr int halfWidth = APP_INIT_WINDOW_WIDTH / 2;
		constexpr int halfHeight = APP_INIT_WINDOW_HEIGHT / 2;
		sprite->SetPosition(halfWidth, halfHeight);
		sprite->SetScale(0.5);
		playerWinSprite = std::shared_ptr<CSimpleSprite>(sprite);
	}

	auto graph = shared_from_this();
	const auto observationState = std::make_shared<ObservationState>(graph);
	const auto golfAimingState = std::make_shared<GolfAimingState>(graph);
	const auto golfShootState = std::make_shared<GolfShootState>(graph);
	const auto holeProgressState = std::make_shared<HoleProgressState>(graph);
	const auto otherPlayerState = std::make_shared<OtherPlayerState>(graph);
	const auto winState = std::make_shared<WinState>(graph);

	{
		auto observationToGolfAim = [this]() -> bool
		{
			return m_isObservationKeyPressed;
		};
		observationState->addTransition(golfAimingState, observationToGolfAim);
	}

	{
		auto golfAimToObservation = [this]() -> bool
		{
			return m_isAimKeyPressed;
		};
		golfAimingState->addTransition(observationState, golfAimToObservation);

		auto golfAimToGolfShoot = [this]() -> bool
		{
			return isShootKeyPressed && charge != 0.0f;
		};
		golfAimingState->addTransition(golfShootState, golfAimToGolfShoot);
	}

	{
		auto golfShootToOtherPlayer = [this]() -> bool
		{
			const auto& playerVelocity = players[0]->getBody()->getLinearVelocity();
			return playerVelocity.magnitudeSquared() < 1.5e-1f;
		};
		golfShootState->addTransition(otherPlayerState, golfShootToOtherPlayer);

		auto golfShootToHoleProgress = [this]() -> bool
		{
			return remainingHoleCountChanged && remainingHoles() != 0;
		};
		golfShootState->addTransition(holeProgressState, golfShootToHoleProgress);

		auto golfShootToWin = [this]() -> bool
		{
			return remainingHoles() == 0;
		};
		golfShootState->addTransition(winState, golfShootToWin);
	}

	{
		auto holeProgressToObservation = [this]() -> bool
		{
			return currentPlayerIndex == 0;
		};
		holeProgressState->addTransition(observationState, holeProgressToObservation);

		auto holeProgressToOtherPlayer = [this]() -> bool
		{
			return currentPlayerIndex == 1;
		};
		holeProgressState->addTransition(otherPlayerState, holeProgressToOtherPlayer);
	}

	{
		auto otherPlayerToObservation = [this]() -> bool
		{
			const auto& playerVelocity = players[1]->getBody()->getLinearVelocity();
			return otherActed && playerVelocity.magnitudeSquared() < 1.5e-1f;
		};
		otherPlayerState->addTransition(observationState, otherPlayerToObservation);

		auto otherPlayerToHoleProgress = [this]() -> bool
		{
			return remainingHoleCountChanged && remainingHoles() != 0;
		};
		otherPlayerState->addTransition(holeProgressState, otherPlayerToHoleProgress);

		auto otherPlayerToWin = [this]() -> bool
		{
			return remainingHoles() == 0;
		};
		otherPlayerState->addTransition(winState, otherPlayerToWin);
	}

	addState(observationState);
	addState(golfAimingState);
	addState(golfShootState);
	addState(otherPlayerState);
	addState(holeProgressState);
	addState(winState);

	switchState(0);
}

void GameplayStateMachine::on_update(float deltaTimeInSecond)
{
	constexpr int observeKey = 'O';
	if (m_isObservationKeyPressed && !App::IsKeyPressed(observeKey))
		m_isObservationKeyPressed = false;
	if (App::IsKeyPressed(observeKey) && !m_isObservationKeyPressed)
		m_isObservationKeyPressed = true;

	constexpr int aimKey = 'O';
	if (m_isAimKeyPressed && !App::IsKeyPressed(aimKey))
		m_isAimKeyPressed = false;
	if (App::IsKeyPressed(aimKey) && !m_isAimKeyPressed)
		m_isAimKeyPressed = true;

	constexpr int shootKey = 'J';
	if (isShootKeyPressed && !App::IsKeyPressed(shootKey))
		isShootKeyPressed = false;
	if (App::IsKeyPressed(shootKey) && !isShootKeyPressed)
		isShootKeyPressed = true;
}
