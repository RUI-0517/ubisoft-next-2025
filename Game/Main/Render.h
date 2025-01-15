#pragma once
#include "App/app.h"
#include "Renderer.h"

namespace Rendering
{
	using Buffer = std::vector<std::unique_ptr<CSimpleSprite>>;
	inline std::unique_ptr<Renderer> RENDERER;
	static size_t THREAD_COUNT = std::thread::hardware_concurrency();

	inline Buffer PIXELS;
	constexpr size_t PIXEL_SIZE = 4;

	inline float TIME_PASSED;

	constexpr float SPHERE_MATERIAL_ID = 0.0f;
	constexpr float PLANE_MATERIAL_ID = 1.0f;

	const Vector3f PLANE_NORMAL{0.0f, 1.0f, 0.0f};

	inline Vector3f SPHERE_CENTER{0.0f, 1.0f, 0.0f};
	inline float SPHERE_RADIUS = 1.0f;

	const Vector3f SKY_COLOR{0.8f};

	void InitializePixels(size_t resolution, size_t pixelSize);
	void UpdatePixels(const Buffer& pixels, const std::vector<Vector4f>& colorBuffer);

	float CalculateDepth(float fov);
	Vector4f RenderScene(const Vector3f& rayOrigin, const Vector3f& rayDirection);
	std::tuple<float, float> IntersectScene(const Vector3f& rayOrigin, const Vector3f& rayDirection);

	float SdSphere(const Vector3f& point, float radius);
	float SdScene(const Vector3f& point);

	float SdCheckerBoard(const Vector3f& point);

	float Union(float d1, float d2);

	float IntersectSphere(const Vector3f& rayOrigin, const Vector3f& rayDirection,
	                      const Vector3f& sphereCenter, float sphereRadius);

	Vector3f CalculateSphereNormal(const Vector3f& hitPoint, const Vector3f& center);
	Vector4f ApplyLighting(const Vector3f& hitPoint, const Vector3f& normal,
	                       const Vector3f& rayDirection, const Vector3f& lightPosition);
	Vector4f ApplyDirectionalLighting(const Vector3f& hitPoint, const Vector3f& normal);
	[[nodiscard]] Vector3f ApplySkyLight(const Vector3f& color, const Vector3f& hitPoint,
	                                     const Vector3f& rayDirection, const Vector3f& normal);
	float CalculateAmbientOcclusion(const Vector3f& hitPoint, const Vector3f& normal);
	float ApplyShadow(const Vector3f& rayOrigin, const Vector3f& rayDirection, float tMin, float tMax, float softness);
	float ApplyFog(float t, float density);

	float SmoothStep(float edge0, float edge1, float x);
	Vector3f Reflect(const Vector3f& in, const Vector3f& normal);
}
