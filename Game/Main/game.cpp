///////////////////////////////////////////////////////////////////////////////
// Filename: GameTest.cpp
// Provides a demo of how to use the API
///////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------

#include "Physics.h"
#include "PhysicsScene.h"

std::unique_ptr<PhysicsScene> PHYSICS_SCENE;

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	PHYSICS_SCENE = std::make_unique<PhysicsScene>();
	PHYSICS_SCENE->Init();
}

//------------------------------------------------------------------------
// update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(const float deltaTime)
{
	const float deltaTimeInSecond = deltaTime / 1000.0f;
	PHYSICS_SCENE->Update(deltaTimeInSecond);
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{
	PHYSICS_SCENE->Render();
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
	PHYSICS_SCENE->Shutdown();
}
