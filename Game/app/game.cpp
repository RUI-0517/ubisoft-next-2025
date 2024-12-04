//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include <stdafx.h>
//------------------------------------------------------------------------
#include <windows.h>
//------------------------------------------------------------------------
#include <chrono>
#include <fstream>
#include <ratio>
#include <App/app.h>

#define ENABLE_SIMD
#include "math/include/Vector.h"

//------------------------------------------------------------------------

// Use ^(?!.*(extern|MSVC)).*reason\s*'\d+' as a regex pattern 
// to locate vectorization warnings

#define RESOURCE_FOLDER ".\\resources\\"
#define RESOURCE_PATH(file) (RESOURCE_FOLDER file)


std::string getCurrentTime()
{
	const auto now = std::chrono::system_clock::now();
	const std::time_t now_time = std::chrono::system_clock::to_time_t(now);
	char buffer[100];
	tm timeInfo;

	if (localtime_s(&timeInfo, &now_time) == 0)
		std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeInfo);
	else return "Invalid Time";

	return buffer;
}

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	constexpr int iterations = 10000000;

	Vector3 a{1.1f, 1.12f, 1.12f};

	Vector3 result{1.0f, 1.12f, 1.0f};
	const auto start = std::chrono::high_resolution_clock::now();

	float k = 0.0f;

	for (int i = 0; i < iterations; ++i)
	{
		result = result.cross(a);
	}

	const auto end = std::chrono::high_resolution_clock::now();
	const auto duration = std::chrono::duration<double, std::nano>(end - start).count();

	std::ofstream log("output.log", std::ios_base::app);
	log << "-------------------------\n";
	log << "Current Time: " << getCurrentTime() << "\n";
	log << "Final Result: " << result << "\n";
	log << "Final Result: " << k << "\n";
	log << "Performed " << iterations << " additions in " << duration << " ns\n";
	log << "Average time per addition: " << (duration / iterations) << " ns\n";
	log.close();
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
