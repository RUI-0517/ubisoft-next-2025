//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include <stdafx.h>
#include <windows.h>
#include <App/app.h>

// Controls whether the math folder uses the scalar or vectorized version 
// of the implementation.
// Must be included before including the math headers, 
// or it should be set as a global flag.
#define ENABLE_SIMD
// Note: In this project, 
// the AVX2 instruction set has been enabled for dot product calculations. 
// However, if this set is not supported by a user's system, 
// you should go to Project -> Game Properties -> C/C++ -> Code Generation 
// and select 'No Enhanced Instruction Set' under the 
// 'Enable Enhanced Instruction Set' option.

// Use ^(?!.*(extern|MSVC)).*reason\s*'\d+' as a regex pattern 
// to locate vectorization warnings

#include "math/include/Vector.h"

//------------------------------------------------------------------------

#define RESOURCE_FOLDER ".\\resources\\"
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
void Update(float deltaTime)
{
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
}
