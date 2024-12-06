///////////////////////////////////////////////////////////////////////////////
// Filename: GameTest.cpp
// Provides a demo of how to use the API
///////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h>
#include <math.h>
//------------------------------------------------------------------------
#include "app\app.h"
//------------------------------------------------------------------------

#include "Vector.h"

#define RESOURCE_FOLDER "..\\Resources\\"
#define RESOURCE_PATH(file) (RESOURCE_FOLDER file)

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(const float deltaTime)
{
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{
	const Vector3f a{static_cast<float>(APP_INIT_WINDOW_WIDTH), static_cast<float>(APP_INIT_WINDOW_HEIGHT), 0.0f};
	const Vector3f b{0.0f, 0.0f, 50.0f};
	const Vector3f result = (a + b) / 2.0f;

	App::Print(result.x, result.y, "Hello World");
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
}
