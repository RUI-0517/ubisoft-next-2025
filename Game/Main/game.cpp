///////////////////////////////////////////////////////////////////////////////
// Filename: GameTest.cpp
// Provides a demo of how to use the API
///////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h>
//------------------------------------------------------------------------
#include "app/app.h"
//------------------------------------------------------------------------

#include "Physics.h"
#include "Render.h"

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	Physics::ACCUMULATED_TIME = 0.0f;
	Physics::WORLD = std::make_unique<World>();
	Physics::WORLD->setGravity(Physics::GRAVITY);

	constexpr size_t width = APP_INIT_WINDOW_WIDTH;
	constexpr size_t height = APP_INIT_WINDOW_HEIGHT;

	Rendering::RENDERER = std::make_unique<Renderer>(width / Rendering::PIXEL_SIZE,
	                                                 height / Rendering::PIXEL_SIZE);

	constexpr size_t resolution = width * height;
	Rendering::PIXELS = std::vector<std::unique_ptr<CSimpleSprite>>();
	Rendering::InitializePixels(resolution, Rendering::PIXEL_SIZE);
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(const float deltaTime)
{
	Rendering::TIME_PASSED += deltaTime / 1000.0f;

	// Fixed Update
	Physics::ACCUMULATED_TIME += deltaTime / 1000.0f;
	if (Physics::ACCUMULATED_TIME >= Physics::FIXED_DELTA_TIME)
	{
		Physics::WORLD->simulate(Physics::FIXED_DELTA_TIME);
		Physics::ACCUMULATED_TIME -= Physics::FIXED_DELTA_TIME;
	}

	// Game Logic Update
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

	// auto uvCoordinate = [](std::vector<Vector4f>& buffer, const size_t index, float u, float v)
	// {
	// 	u -= 0.5f;
	// 	v -= 0.5f;
	// 	buffer[index] = {u, v, 0, 99};
	// };

	// TODO: using Vector2f instead individual component
	auto rayMarching = [](std::vector<Vector4f>& buffer, const size_t index, float u, float v)
	{
		u -= 0.5f;
		v -= 0.5f;

		constexpr float fov = 90.0f;
		const float depth = Rendering::CalculateDepth(fov);

		const Vector3f rayOrigin = {0, 0, -8.0f};
		const Vector3f rayDirection = Rendering::CalculateRayDirection(u, v, depth);

		// TODO: Using geom transform
		const Vector3f sphereCenter{0.0f, 0.0f, 0.0f};
		constexpr float sphereRadius = 1.0f;

		const float t = Rendering::IntersectSphere(rayOrigin, rayDirection, sphereCenter, sphereRadius);
		const bool hasIntersected = t > 0.0f;

		Vector4f fragColor;
		if (hasIntersected)
			fragColor = {1.0f, 0.0f, 0.0f, 1.0f};
		else
			fragColor = {0.9f, 0.9f, 0.9f, 1.0f};

		buffer[index] = fragColor;
	};

	// Execute the UV coordinate computation for all render threads
	Rendering::RENDERER->each(Rendering::THREAD_COUNT, rayMarching);

	// Synchronize the renderer's output buffer to the pixel textures
	// This updates PIXELS with the latest data from the renderer's buffer
	Rendering::UpdatePixels(Rendering::PIXELS, Rendering::RENDERER->getBuffer());

	// Render each pixel on the screen
	// Due to OpenGL's requirement for rendering on the main thread,
	// the drawing process cannot be multithreaded
	for (const auto& pixel : Rendering::PIXELS)
		pixel->Draw();

	App::Print(result.x - 10, result.y - 10, "+");
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
}
