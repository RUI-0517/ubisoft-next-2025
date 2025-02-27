///////////////////////////////////////////////////////////////////////////////
// Filename: GameTest.cpp
// Provides a demo of how to use the API
///////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------

#include "RenderScene.h"
#include <array>

#include "GameScene.h"
#include "PhysicsResolveScene.h"

std::array<std::shared_ptr<Scene>, 3> SCENES;

size_t CURRENT_SCENE_INDEX;

static void HandleUserInput();
static void SwitchScene(size_t index);

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	SCENES[0] = std::make_shared<PhysicsResolveScene>();
	SCENES[1] = std::make_shared<RenderScene>();
	SCENES[2] = std::make_shared<GameScene>();

	SwitchScene(2);
}

//------------------------------------------------------------------------
// update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(const float deltaTime)
{
	HandleUserInput();

	const float deltaTimeInSecond = deltaTime / 1e3f;
	SCENES[CURRENT_SCENE_INDEX]->Update(deltaTimeInSecond);
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{
	SCENES[CURRENT_SCENE_INDEX]->Render();
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
	SCENES[CURRENT_SCENE_INDEX]->Shutdown();
}

void HandleUserInput()
{
	// TODO: DONING GAMEPLAY
	// if (App::IsKeyPressed(VK_SPACE))
	// {
	// 	++CURRENT_SCENE_INDEX %= SCENES.size();
	// 	SwitchScene(CURRENT_SCENE_INDEX);
	// }
}

void SwitchScene(const size_t index)
{
	if (SCENES[CURRENT_SCENE_INDEX] != nullptr) SCENES[CURRENT_SCENE_INDEX]->Shutdown();
	CURRENT_SCENE_INDEX = index;
	SCENES[CURRENT_SCENE_INDEX]->Init();
}
