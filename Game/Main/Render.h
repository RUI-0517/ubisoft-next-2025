#pragma once
#include "App/app.h"
#include "Renderer.h"

namespace Rendering
{
	using Buffer = std::vector<std::unique_ptr<CSimpleSprite>>;
	inline std::unique_ptr<Renderer> RENDERER;
	static size_t THREAD_COUNT = std::thread::hardware_concurrency() - 1;

	inline Buffer PIXELS;
	constexpr size_t PIXEL_SIZE = 4;

	inline float TIME_PASSED;

	constexpr float SPHERE_MATERIAL_ID = 0.0f;
	constexpr float PLANE_MATERIAL_ID = 1.0f;

	const Vector3f PLANE_NORMAL{0.0f, 1.0f, 0.0f};

	inline Vector3f SPHERE_CENTER{0.0f, 1.0f, 0.0f};
	inline float SPHERE_RADIUS = 1.0f;

	const Vector3f SKY_COLOR{0.85f, 0.8f, 0.75f};
	const Vector3f DIRECTIONAL_LIGHT_DIR{-0.75f, 0.5f, -0.5f};
	const Vector3f DIRECTIONAL_LIGHT_COLOR{0.95f, 0.85f, 0.8f};

	void InitializePixels(size_t resolution, size_t pixelSize);
	void UpdatePixels(const Buffer& pixels, const std::vector<Vector4f>& colorBuffer);

	[[nodiscard]] float CalculateDepth(float fov);
	[[nodiscard]] Vector4f RenderScene(const Vector3f& rayOrigin, const Vector3f& rayDirection);
	[[nodiscard]] std::tuple<float, float> TraceRay(const Vector3f& rayOrigin, const Vector3f& rayDirection);

	[[nodiscard]] float SdSphere(const Vector3f& point, float radius);
	[[nodiscard]] float SdScene(const Vector3f& point);

	[[nodiscard]] float SdCheckerBoard(const Vector3f& point);

	[[nodiscard]] constexpr float Union(float d1, float d2);

	[[nodiscard]] float IntersectSphere(const Vector3f& rayOrigin, const Vector3f& rayDirection);
	[[nodiscard]] float IntersectScene(const Vector3f& rayOrigin, const Vector3f& rayDirection);

	[[nodiscard]] Vector3f CalculateNormal(const Vector3f& hitPoint);
	[[nodiscard]] Vector3f CalculateSphereNormal(const Vector3f& hitPoint, const Vector3f& center);

	[[nodiscard]] Vector3f ApplyDirectionalLighting(const Vector3f& color, const Vector3f& rayDirection,
	                                                const Vector3f& hitPoint, const Vector3f& normal);
	[[nodiscard]] Vector3f ApplySkyLight(const Vector3f& color, const Vector3f& rayDirection,
	                                     const Vector3f& hitPoint, const Vector3f& normal);
	[[nodiscard]] float ApplyShadow(const Vector3f& rayOrigin, const Vector3f& rayDirection, float tMin, float tMax,
	                                float softness);
	[[nodiscard]] float ApplyFog(float t, float density);

	[[nodiscard]] float SmoothStep(float edge0, float edge1, float x);
	[[nodiscard]] Vector3f Reflect(const Vector3f& in, const Vector3f& normal);

	[[nodiscard]] float CalculateAmbientOcclusion(const Vector3f& hitPoint, const Vector3f& normal);
	[[nodiscard]] float CalculateFresnel(const Vector3f& in, const Vector3f& normal);
}
