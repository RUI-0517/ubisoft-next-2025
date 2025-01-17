#include "stdafx.h"
#include "App/app.h"
#include "RayMarchingRenderer.h"
#include "Resources.h"
#include <algorithm>

size_t RayMarchingRenderer::m_threadCounts = std::thread::hardware_concurrency();

RayMarchingRenderer::RayMarchingRenderer(const size_t width, const size_t height)
	: Renderer(width / pixelSize, height / pixelSize)
{
	m_pixels = std::vector<std::unique_ptr<CSimpleSprite>>();
	initialize_pixels(width, height);
}

void RayMarchingRenderer::update()
{
	// ray info
	const Vector3f rayOrigin = {0.0f, 4.0f, -10.0f};

	// camera settings
	const Vector3f& cameraOrigin = rayOrigin;
	const Vector3f cameraLookAt{0.0f, 0.0f, 0.0f};
	constexpr float fov = 90.0f;
	const float focalLength = calculate_depth(fov);

	// camera coordinate system
	const Vector3f forward = (cameraLookAt - cameraOrigin).normalize();
	const Vector3f right = Vector3f{0.0f, 1.0f, 0.0f}.cross(forward).normalize();
	const Vector3f up = forward.cross(right);

	// TODO: Camera model
	auto rayMarching = [&](std::vector<Vector4f>& buffer, const size_t index, float u, float v)
	{
		// center uv
		u -= 0.5f;
		v -= 0.5f;

		const Vector3f rayDirection = Vector3f{u * right + v * up + focalLength * forward}.normalize();

		// render scene
		Vector4f fragColor = render_scene(rayOrigin, rayDirection);

		// gamma correction
		fragColor = fragColor.pow(1.0f / 2.2f);

		// write color back to color buffer
		buffer[index] = fragColor;
	};

	// Execute the ray marching for all render threads
	each(m_threadCounts, rayMarching);
}

void RayMarchingRenderer::Render()
{
	update_pixels();

	// Render each pixel on the screen
	// Due to OpenGL's requirement for rendering on the main thread,
	// the drawing process cannot be multithreaded
	for (const auto& pixel : m_pixels)
		pixel->Draw();
}

void RayMarchingRenderer::shutdown()
{
}

void RayMarchingRenderer::initialize_pixels(const size_t width, const size_t height)
{
	const size_t resolution = width * height;
	const size_t size = resolution / (pixelSize * pixelSize);
	m_pixels.resize(size);

	constexpr float pixelSizeInFloat = static_cast<float>(pixelSize);

	for (size_t index = 0; index < size; ++index)
	{
		const size_t indexX = index % (width / pixelSize);
		const size_t indexY = index / (height / pixelSize);

		const float x = static_cast<float>(indexX) * pixelSizeInFloat + pixelSizeInFloat / 2.0f;
		const float y = static_cast<float>(indexY) * pixelSizeInFloat + pixelSizeInFloat / 2.0f;

		CSimpleSprite* texture = App::CreateSprite(RESOURCE_PATH("Opacity Texture.png"), 100, 1);
		texture->SetPosition(x, y);
		// TODO: Remove as alpha will be handled in raymarching
		texture->SetFrame(99);
		m_pixels[index] = std::unique_ptr<CSimpleSprite>(texture);
	}
}

void RayMarchingRenderer::update_pixels()
{
	// m_pixels, getBuffer()
	const std::vector<Vector4f>& colorBuffer = getBuffer();

	if (m_pixels.size() != colorBuffer.size())
		throw std::invalid_argument("The sizes of pixels and colorBuffer must be the same.");

	const size_t threadCount = std::thread::hardware_concurrency();
	const size_t chunkSize = m_pixels.size() / threadCount;

	auto worker = [&](const size_t start, const size_t end)
	{
		for (size_t i = start; i < end; ++i)
		{
			const auto& color = colorBuffer[i];
			m_pixels[i]->SetColor(color.x, color.y, color.z);
		}
	};

	std::vector<std::thread> threads;

	for (size_t t = 0; t < threadCount; ++t)
	{
		size_t start = t * chunkSize;
		size_t end = (t == threadCount - 1) ? m_pixels.size() : start + chunkSize;
		threads.emplace_back(worker, start, end);
	}

	for (auto& thread : threads)
		thread.join();
}

float RayMarchingRenderer::calculate_depth(const float fov)
{
	const float halfFov = fov / 2.0f;
	const float halfFovRadians = halfFov * (PI / 180.0f); // Convert degrees to radians
	return 1.0f / tan(halfFovRadians);
}

Vector4f RayMarchingRenderer::render_scene(const Vector3f& rayOrigin, const Vector3f& rayDirection)
{
	Vector3f color = m_skyColor;

	auto [t, materialId] = trace_ray(rayOrigin, rayDirection);
	const bool hasIntersected = t > 0.0f;

	if (hasIntersected)
	{
		const Vector3f hitPoint = rayOrigin + t * rayDirection;

		const bool isGround = materialId == PLANE;

		// TODO: Using geom transform
		// Calculate Normal
		const Vector3f normal = isGround ? m_planeNormal : calculate_normal(hitPoint);

		if (isGround)
		{
			const float x = sd_checkerboard(hitPoint);
			color = Vector3f{0.15f} + x * Vector3f{0.05f};
		}
		else if (materialId == GREEN)
		{
			color = Vector3f{0.0f, 0.5f, 0.0f};
		}
		else if (materialId == RED)
		{
			color = Vector3f{0.5f, 0.0f, 0.0f};
		}

		Vector3f lighting{0.0f};

		auto directionalLight = ApplyDirectionalLighting(color, rayDirection, hitPoint, normal);

		lighting += directionalLight;

		Vector3f skyLight = ApplySkyLight(color, rayDirection, hitPoint, normal);
		float ambientOcclusion = CalculateAmbientOcclusion(hitPoint, normal);

		lighting += skyLight * ambientOcclusion;

		float fogFactor = ApplyFog(t, 1e-5f);

		color = lighting.lerp(m_skyColor, fogFactor);
	}

	Vector4f fragColor = color;

	return fragColor;
}

std::pair<float, MaterialId> RayMarchingRenderer::trace_ray(const Vector3f& rayOrigin,
                                                            const Vector3f& rayDirection) const
{
	float t = -1.0f;
	MaterialId materialId = UNDEFINED;

	const float tFloor = -rayOrigin.dot(m_planeNormal) / rayDirection.dot(m_planeNormal);
	if (tFloor > 0.0f)
	{
		t = tFloor;
		// tMax = min(tMax, t);
		materialId = PLANE;
	}

	auto [tScene ,tSceneMaterialId] = IntersectScene(rayOrigin, rayDirection);
	if (tScene > 1.0f && (t < 0.0f || tScene < t))
	{
		t = tScene;
		materialId = tSceneMaterialId;
	}

	return {t, materialId};
}

// Signed distance function for a sphere
float RayMarchingRenderer::sd_sphere(const Vector3f& point, const float radius)
{
	return point.magnitude() - radius;
}

float RayMarchingRenderer::sd_scene(const Vector3f& point) const
{
	float result = 1.0f;


	for (const auto& object : m_objects)
	{
		const float currentResult = object->evaluate(point).first;
		result = op_union(result, currentResult);
	}

	return result;
}

std::pair<float, MaterialId> RayMarchingRenderer::sd_scene_material(const Vector3f& point) const
{
	std::pair result{1.0f, UNDEFINED};

	for (const auto& object : m_objects)
	{
		const auto& currentResult = object->evaluate(point);
		result = op_union_tuple(result, currentResult);
	}

	return result;
}

float RayMarchingRenderer::sd_checkerboard(const Vector3f& point)
{
	const int x = static_cast<int>(round(point.x + 0.5f));
	const int z = static_cast<int>(round(point.z + 0.5f));
	return static_cast<float>((x ^ z) & 1);
}

float RayMarchingRenderer::op_union(const float d1, const float d2)
{
	return d1 < d2 ? d1 : d2;
}

const std::pair<float, MaterialId>& RayMarchingRenderer::op_union_tuple(const std::pair<float, MaterialId>& t1,
                                                                        const std::pair<float, MaterialId>& t2)
{
	return std::get<0>(t1) < std::get<0>(t2) ? t1 : t2;
}


std::pair<float, MaterialId> RayMarchingRenderer::IntersectScene(const Vector3f& rayOrigin,
                                                                 const Vector3f& rayDirection) const
{
	float t = 0.0f;
	for (size_t i = 0; i < 128; i++)
	{
		// Iterate for a maximum of 100 steps
		Vector3f p = rayOrigin + rayDirection * t;
		auto [d, materialId] = sd_scene_material(p);
		if (d < 0.001f)
		{
			// If the distance is small enough, we have an intersection
			return {t, materialId};
		}

		// Move along the ray by the distance
		t += d;
		if (t > 100.0)
		{
			break;
		}
	}

	return {-1.0f, UNDEFINED}; // No intersection found
}

// Calculate the SDF normal using forward differences for efficiency, 
// requiring 4 sd_scene evaluations instead of 6 as in central differences.
Vector3f RayMarchingRenderer::calculate_normal(const Vector3f& hitPoint) const
{
	// Equivalent to the formula: f(x) = f(x + h) - f(x)
	// Optimized by introducing more constants to better leverage SIMD broadcast calculations.

	constexpr float epsilon = 1e-3f;
	constexpr float epsilonInv = 1 / epsilon;
	const Vector3f offsetX{epsilon, 0, 0};
	const Vector3f offsetY{0, epsilon, 0};
	const Vector3f offsetZ{0, 0, epsilon};
	const float sdfAtPoint = sd_scene(hitPoint);

	const float partialX = sd_scene(hitPoint + offsetX) - sdfAtPoint;
	const float partialY = sd_scene(hitPoint + offsetY) - sdfAtPoint;
	const float partialZ = sd_scene(hitPoint + offsetZ) - sdfAtPoint;

	Vector3f gradient{partialX, partialY, partialZ};

	// This form allows a SIMD implementation to leverage broadcast optimizations.
	gradient *= epsilonInv;

	return gradient.normalize();
}

Vector3f RayMarchingRenderer::ApplyDirectionalLighting(const Vector3f& color, const Vector3f& rayDirection,
                                                       const Vector3f& hitPoint, const Vector3f& normal) const
{
	const Vector3f lightDir = m_directionalLightDirection.normalize();
	const float diffuseFactor = std::clamp(normal.dot(lightDir), 0.0f, 1.0f);
	const float softShadow = ApplyShadow(hitPoint, lightDir, 0.02f, 2.5f, 8);

	// Direction from point to camera (origin)
	const Vector3f viewDir = -rayDirection.normalize();
	// Halfway vector between light and view
	const Vector3f halfDir = (lightDir + viewDir).normalize();

	// Specular component
	constexpr float shininess = 16.0;
	float specularFactor = std::pow(max(normal.dot(halfDir), 0.0f), shininess);
	specularFactor *= diffuseFactor;

	const float fresnelReflectance = CalculateFresnel(lightDir, halfDir);

	const Vector3f diffuseColor = color.hadamard(m_DirectionalLightColor) * diffuseFactor * softShadow;
	const float specularColor = specularFactor * fresnelReflectance * 4.0f;

	return diffuseColor + specularColor;
}

Vector3f RayMarchingRenderer::ApplySkyLight(const Vector3f& color, const Vector3f& rayDirection,
                                            const Vector3f& hitPoint, const Vector3f& normal) const
{
	// Calculate diffuse lighting with an offset to brighten downward-facing surfaces,
	// simulating bounced light and avoiding complete darkness.
	constexpr float bias = 0.5f; // Offset to brighten downward-facing surfaces
	constexpr float scale = 0.5f; // Scale factor for normal.y contribution

	// Calculate diffuse lighting with clamping to ensure valid range [0, 1]
	const float diffuse = sqrt(std::clamp(bias + scale * normal.y, 0.0f, 1.0f));

	const Vector3f reflect = Reflect(rayDirection, normal);
	const float smoothSpecularFactor = SmoothStep(-0.2f, 0.2f, reflect.y);

	const float fresnelReflectance = CalculateFresnel(-rayDirection, normal);
	const float reflection = ApplyShadow(hitPoint, reflect, 0.02f, 2.0f, 32);

	const Vector3f diffuseColor = color.hadamard(m_skyColor * diffuse * 0.5f);
	const Vector3f specularColor = (smoothSpecularFactor * fresnelReflectance * reflection * 2.0f) * m_skyColor;

	return diffuseColor + specularColor;
}

float RayMarchingRenderer::CalculateAmbientOcclusion(const Vector3f& hitPoint, const Vector3f& normal) const
{
	float value = 0.0f;
	float attenuationFactor = 1.0f;
	constexpr size_t maxIteration = 5;

	for (size_t i = 0; i < maxIteration; ++i)
	{
		const float minDistance = 0.025f * static_cast<float>(i);
		const Vector3f samplePoint = hitPoint + minDistance * normal;
		const auto distance = sd_scene(samplePoint);

		// Sample along the normal to check for occlusion.
		// If the SDF value at samplePoint is smaller than sampleDistance, 
		// it indicates a closer surface, meaning the hit point is occluded.
		value += (minDistance - distance) * attenuationFactor;

		attenuationFactor *= 0.8f;
	}

	// Adjust the parameters to achieve the desired visual effect.
	const float normalFactor = 0.5f + 0.5f * normal.y;
	return std::clamp(exp(-10.0f * value), 0.0f, 1.0f) * normalFactor;
}

// remark: softness higher, clearer, harder
float RayMarchingRenderer::ApplyShadow(const Vector3f& rayOrigin, const Vector3f& rayDirection,
                                       const float tMin, const float tMax, const float softness) const
{
	float result = 1.0f;

	float t = tMin;
	for (size_t i = 0; i < 128; i++)
	{
		Vector3f samplePoint = rayOrigin + rayDirection * t;
		const float currentDistance = sd_scene(samplePoint);

		if (currentDistance < 1e-3f) return 0.0f;

		// Move along the ray by the distance
		t += currentDistance;

		// The following formula adjusts the shadow softness based on distance:
		// The closer the ray is to the hit point, the darker the penumbra,
		// creating a smoother shadow edge.
		result = min(result, softness * currentDistance / t);

		if (t >= tMax)
		{
			// If we've moved too far, stop
			break;
		}
	}
	return result;
}

float RayMarchingRenderer::ApplyFog(const float t, const float density)
{
	return 1.0f - exp(-density * t * t * t);
}

float RayMarchingRenderer::SmoothStep(const float edge0, const float edge1, const float x)
{
	const float t = std::clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
	return t * t * (3.0f - 2.0f * t);
}

Vector3f RayMarchingRenderer::Reflect(const Vector3f& in, const Vector3f& normal)
{
	return in - 2.0f * in.dot(normal) * normal;
}

// schlick fresnel approximation
float RayMarchingRenderer::CalculateFresnel(const Vector3f& in, const Vector3f& normal)
{
	constexpr float baseReflectivity = 0.04f;
	constexpr float oneMinusBaseReflectivity = 1.0f - baseReflectivity;
	float cosTheta = in.dot(normal);
	cosTheta = std::clamp(cosTheta, 0.0f, 1.0f);
	const float fresnelReflectance = baseReflectivity + oneMinusBaseReflectivity * pow((1.0f - cosTheta), 5.0f);
	return fresnelReflectance;
}
