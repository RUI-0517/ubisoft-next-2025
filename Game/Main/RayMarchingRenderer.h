#pragma once
#include "App/SimpleSprite.h"
#include "Transform.h"
#include <Renderer.h>

#include "RayMarchingObject.h"

class RayMarchingRenderer final : public Renderer
{
	// pixelSize should be a multiple of APP_INIT_WIDTH for proper alignment.
	// TODO: Handle cases where pixelSize is not perfectly divisible by APP_INIT_WIDTH.
	static constexpr size_t pixelSize = 4;

	// Alias for the buffer containing sprite pointers
	using Buffer = std::vector<std::unique_ptr<CSimpleSprite>>;

	// Color buffer to hold pixel data
	Buffer m_pixels;

	// Renderer instance for parallel processing of the color buffer
	// std::unique_ptr<Renderer> m_renderer;

	// Number of threads available for parallel processing
	static size_t m_threadCounts;

	// Global lighting settings
	const Vector3f m_skyColor{0.85f, 0.8f, 0.75f};
	const Vector3f m_directionalLightDirection{-0.75f, 0.5f, -0.5f};
	const Vector3f m_DirectionalLightColor{0.95f, 0.85f, 0.8f};

	std::vector<std::shared_ptr<RayMarchingObject>> m_objects;

	static constexpr float SPHERE_MATERIAL_ID = 0.0f;
	static constexpr float PLANE_MATERIAL_ID = 1.0f;

	// Speical every scene has a plane
	const Vector3f m_planeNormal{0.0f, 1.0f, 0.0f};
	// Vector3f SPHERE_CENTER{0.0f, 1.0f, 0.0f};

	float SPHERE_RADIUS = 1.0f;

public:
	RayMarchingRenderer(size_t width, size_t height);

	void update();
	void Render();
	static void shutdown();

	template <typename T, typename... Args>
	void addObject(Args&&... args)
	{
		m_objects.emplace_back(std::make_shared<T>(std::forward<Args>(args)...));
	}

private:
#pragma region Internals
	void initialize_pixels(size_t width, size_t height);
	void update_pixels();

	[[nodiscard]] static float calculate_depth(float fov);
	[[nodiscard]] Vector4f render_scene(const Vector3f& rayOrigin, const Vector3f& rayDirection);
	[[nodiscard]] std::tuple<float, float> trace_ray(const Vector3f& rayOrigin, const Vector3f& rayDirection);

	[[nodiscard]] static float sd_sphere(const Vector3f& point, float radius);
	[[nodiscard]] float sd_scene(const Vector3f& point) const;

	[[nodiscard]] static float sd_checkerboard(const Vector3f& point);

	[[nodiscard]] static float op_union(float d1, float d2);

	[[nodiscard]] float IntersectScene(const Vector3f& rayOrigin, const Vector3f& rayDirection);

	[[nodiscard]] Vector3f calculate_normal(const Vector3f& hitPoint);

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
#pragma endregion
};
