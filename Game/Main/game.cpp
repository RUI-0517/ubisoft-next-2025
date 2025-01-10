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

#include <sstream>

#include "Physics.h"
#include "Render.h"
#include "Matrix.h"
#include "PlaneGeometry.h"

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

	// const Vector4f vector = {0.5f, 0.5f, 0.0f, 1.0f};
	// const Matrix4f translateMatrix = Matrix4f::translate(2.0f);
	// const Matrix4f scaleMatrix = Matrix4f::scale(2.0f);
	// Vector4f result = vector * (translateMatrix * scaleMatrix);

	Body planeBody{1.0f};
	planeBody.transform.scale = Vector3f{5.0f};
	planeBody.transform.rotation = Vector4f{0.4545195f, 0.1227877f, 0.5416752f, 0.6963643f};

	PlaneGeometry planeGeom{2.0f};
	planeGeom.attachBody(std::make_shared<Body>(planeBody));

	Vector3f supportPoint = planeGeom.getSupportPoint(Vector3f{-1.0f, -1.0f, 1.0f});

	// Quaternion q1{0.7071f, 0.0f, 0.7071f, 0.0f};
	// Quaternion q2{0.7071f, 0.7071f, 0.0f, 0.0f};
	Quaternion q1{1, 0, 0, 0};
	Quaternion q2{0, 1, 0, 0};
	Quaternion result = q1.rotate(q2);
	Quaternion result2 = q2.rotate(q1);

	// TODO: Radian or Degree
	Quaternion q3 = Quaternion::eulerToQuaternion(30, 50, 90);

}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(const float deltaTime)
{
	const float deltaTimeInSecond = deltaTime / 1000.0f;
	Rendering::TIME_PASSED += deltaTimeInSecond;

	// Fixed Update
	Physics::ACCUMULATED_TIME += deltaTimeInSecond;
	if (Physics::ACCUMULATED_TIME >= Physics::FIXED_DELTA_TIME)
	{
		Physics::WORLD->simulate(Physics::FIXED_DELTA_TIME);
		Physics::ACCUMULATED_TIME -= Physics::FIXED_DELTA_TIME;
	}

	const std::shared_ptr<Body>& sphereBody = Physics::WORLD->bodies[1];
	Transform& sphereTransform = sphereBody->transform;
	// sphereBody->setKinematic();
	// sphereTransform.position.y = 1.0f;

	constexpr float velocityThreshold = 1.0f;

	if (sphereTransform.position.y < 1.0f && sphereTransform.position.y >= 0.0f)
	{
		Vector3f& velocity = sphereBody->getLinearVelocity();

		if (std::fabs(velocity.y) < velocityThreshold)
		{
			velocity.y = 0.0f;
			sphereTransform.position.y = 1.0f;
		}
		else velocity.y *= -0.9f;
	}

	Rendering::SPHERE_CENTER = sphereTransform.position;

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
		const Vector3f right = Vector3f{0.0f, 1.0f, 0.0f}.cross(forward).normalize();
		const Vector3f up = forward.cross(right);

		const Vector3f rayDirection = Vector3f{u * right + v * up + focalLength * forward}.normalize();

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

	const std::shared_ptr<Body>& sphereBody = Physics::WORLD->bodies[1];
	const Vector3f& velocity = sphereBody->getLinearVelocity();
	const Vector3f& position = sphereBody->transform.position;

	std::ostringstream os;
	os << velocity;
	App::Print(result.x - 10, result.y - 10, os.str().c_str(), 0, 0, 0);
	os.str("");
	os.clear();
	os << position;
	App::Print(result.x - 10, result.y - 50, os.str().c_str(), 0, 0, 0);
	App::Print(result.x - 10, result.y - 100, std::to_string(Physics::ACCUMULATED_TIME).c_str(), 0, 0, 0);
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
}
