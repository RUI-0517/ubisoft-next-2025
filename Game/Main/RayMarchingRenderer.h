#pragma once
#include "App/SimpleSprite.h"
#include "RayMarchingObject.h"
#include <Renderer.h>

#include "Camera.h"

class RayMarchingRenderer final : public Renderer
{
	Camera m_camera;

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

	// Speical every scene has a plane
	const Vector3f m_planeNormal{0.0f, 1.0f, 0.0f};

public:
	RayMarchingRenderer(size_t width, size_t height);

	void update(float deltaTimeInSecond);
	void Render();
	static void shutdown();

	Camera& getCamera();

	template <typename T, typename= std::enable_if_t<std::is_base_of_v<RayMarchingObject, T>>, typename... Args>
	void addRenderObject(Args&&... args)
	{
		m_objects.emplace_back(std::make_shared<T>(std::forward<Args>(args)...));
	}

	void removeRenderObject(const std::shared_ptr<Geometry>& geometry);

private:
#pragma region Internals
	using SdfPair = std::pair<float, MaterialId>;

	void initialize_pixels(size_t width, size_t height);
	void update_pixels();

	[[nodiscard]] Vector4f render_scene(const Vector3f& rayOrigin, const Vector3f& rayDirection) const;
	[[nodiscard]] SdfPair trace_ray(const Vector3f& rayOrigin, const Vector3f& rayDirection) const;

	[[nodiscard]] static float sd_sphere(const Vector3f& point, float radius);
	[[nodiscard]] float sd_scene(const Vector3f& point) const;
	[[nodiscard]] SdfPair sd_scene_material(const Vector3f& point) const;

	[[nodiscard]] static float sd_checkerboard(const Vector3f& point);

	[[nodiscard]] static float op_union(float d1, float d2);
	[[nodiscard]] static const std::pair<float, MaterialId>& op_union_tuple(const SdfPair& t1, const SdfPair& t2);

	[[nodiscard]] SdfPair IntersectScene(const Vector3f& rayOrigin, const Vector3f& rayDirection) const;

	[[nodiscard]] Vector3f calculate_normal(const Vector3f& hitPoint) const;

	[[nodiscard]] Vector3f apply_directional_lighting(const Vector3f& color, const Vector3f& rayDirection,
	                                                  const Vector3f& hitPoint, const Vector3f& normal) const;
	[[nodiscard]] Vector3f apply_sky_light(const Vector3f& color, const Vector3f& rayDirection,
	                                       const Vector3f& hitPoint, const Vector3f& normal) const;
	[[nodiscard]] float apply_shadow(const Vector3f& rayOrigin, const Vector3f& rayDirection, float tMin, float tMax,
	                                 float softness) const;
	[[nodiscard]] static float apply_fog(float t, float density);

	[[nodiscard]] static float smooth_step(float edge0, float edge1, float x);
	[[nodiscard]] static Vector3f reflect(const Vector3f& in, const Vector3f& normal);

	[[nodiscard]] float calculate_ambient_occlusion(const Vector3f& hitPoint, const Vector3f& normal) const;
	[[nodiscard]] static float calculate_fresnel(const Vector3f& in, const Vector3f& normal);
#pragma endregion
};
