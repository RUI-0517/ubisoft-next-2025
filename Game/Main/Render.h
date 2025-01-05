#pragma once
#include "App/app.h"
#include "Renderer.h"

namespace Rendering
{
	using Buffer = std::vector<std::unique_ptr<CSimpleSprite>>;
	inline std::unique_ptr<Renderer> RENDERER;
	static size_t THREAD_COUNT = std::thread::hardware_concurrency();

	inline Buffer PIXELS;
	constexpr size_t PIXEL_SIZE = 8;

	inline float TIME_PASSED;

	void InitializePixels(size_t resolution, size_t pixelSize);
	void UpdatePixels(const Buffer& pixels, const std::vector<Vector4f>& colorBuffer);

	float CalculateDepth(float fov);
	Vector3f CalculateRayDirection(float u, float v, float depth);
	float SdSphere(const Vector3f& p, float s);
	float IntersectSphere(const Vector3f& rayOrigin, const Vector3f& rayDirection,
	                      const Vector3f& sphereCenter, float sphereRadius);

	Vector3f CalculateSphereNormal(const Vector3f& hitPoint, const Vector3f& center);
	Vector4f ApplyLighting(const Vector3f& hitPoint, const Vector3f& normal,
	                       const Vector3f& rayDirection, const Vector3f& lightPosition);

	Vector4f AcesFilm(const Vector4f& color);
}
