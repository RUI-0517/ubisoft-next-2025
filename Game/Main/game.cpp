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
	// Physics
	Physics::ACCUMULATED_TIME = 0.0f;
	Physics::WORLD = std::make_unique<World>();
	Physics::WORLD->setGravity(Physics::GRAVITY);

	// Rendering
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

	// Rendering Logic
	// TODO: using Vector2f instead individual component
	auto rayMarching = [](std::vector<Vector4f>& buffer, const size_t index, float u, float v)
	{
		// center uv
		u -= 0.5f;
		v -= 0.5f;

		// ray info
		const Vector3f rayOrigin = {0.0f, 2.0f, -10.0f};

		// camera settings
		const Vector3f& cameraOrigin = rayOrigin;
		const Vector3f cameraLookAt{0.0f, 0.0f, 0.0f};
		constexpr float fov = 90.0f;
		const float focalLength = Rendering::CalculateDepth(fov);

		// camera coordinate system
		const Vector3f forward = (cameraLookAt - cameraOrigin).normalize();
		const Vector3f right = Vector3f(0.0f, 1.0f, 0.0f).cross(forward).normalize();
		const Vector3f up = forward.cross(right);

		const Vector3f rayDirection = Vector3f(u * right + v * up + focalLength * forward).normalize();

		// render scene
		Vector4f fragColor = Rendering::RenderScene(rayOrigin, rayDirection);

		// gamma correction
		fragColor = fragColor.pow(1.0f / 2.2f);

		// write color back to color buffer
		buffer[index] = fragColor;
	};

	// Execute the ray marching for all render threads
	Rendering::RENDERER->each(Rendering::THREAD_COUNT, rayMarching);
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
