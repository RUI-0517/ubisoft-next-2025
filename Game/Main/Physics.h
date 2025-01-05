#pragma once
#include <memory>
#include "World.h"

namespace Physics
{
	constexpr float FIXED_DELTA_TIME = 0.02f;
	static Vector3f GRAVITY = {0, -9.81f, 0};

	inline std::unique_ptr<World> WORLD;
	inline float ACCUMULATED_TIME;
};
