///////////////////////////////////////////////////////////////////////////////
// Filename: GameTest.cpp
// Provides a demo of how to use the API
///////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------

#include "Physics.h"
#include "PhysicsScene.h"
#include "RenderScene.h"
#include "App/app.h"
#include <array>

#include "GameScene.h"

std::array<std::shared_ptr<Scene>, 3> SCENES;

// std::shared_ptr<PhysicsScene> PHYSICS_SCENE;
// std::shared_ptr<RenderScene> RENDER_SCENE;
// std::shared_ptr<GameScene> GAME_SCENE;
size_t CURRENT_SCENE_INDEX;

static void HandleUserInput();
static void SwitchScene(size_t index);

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	// const Vector3f currentPosition{7.75f, 0.0f, 11.5f};
	// std::vector<Vector3f> nodes;
	// nodes.emplace_back(2.0f, 0.0f, -8.25f);
	// nodes.emplace_back(-7.25f, 0.0f, 6.5f);
	// nodes.emplace_back(-7.0f, 0.0f, -10.25f);
	// nodes.emplace_back(-7.0f, 0.0f, -10.25f);
	// nodes.emplace_back(5.75f, 0.0f, 1.75f);
	//
	// const auto nodePtr = std::make_shared<std::vector<Vector3f>>(nodes);
	// const Solver solver;
	//
	// for (size_t i = 0; i < 4; ++i)
	// {
	// 	const auto solution = solver.Solve(currentPosition, nodePtr);
	//
	// 	float distance = 0.0f;
	// 	distance += currentPosition.distance(nodes[0]);
	//
	// 	for (size_t i = 1; i < solution.size(); i++)
	// 	{
	// 		distance += nodes[i].distance(nodes[i - 1]);
	// 	}
	//
	// 	App::Print(0, 0, std::to_string(distance).c_str());
	// }

	SCENES[0] = std::make_shared<PhysicsScene>();
	SCENES[1] = std::make_shared<RenderScene>();
	SCENES[3] = std::make_shared<GameScene>();

	SwitchScene(3);
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
