#include "stdafx.h"
#include "Render.h"
#include "Resources.h"
#include <algorithm>

namespace Rendering
{
	void InitializePixels(const size_t resolution, const size_t pixelSize)
	{
		const size_t size = resolution / (pixelSize * pixelSize);
		PIXELS.resize(size);

		const float pixelSizeInFloat = static_cast<float>(pixelSize);

		for (size_t index = 0; index < size; ++index)
		{
			const size_t indexX = index % (APP_INIT_WINDOW_WIDTH / pixelSize);
			const size_t indexY = index / (APP_INIT_WINDOW_WIDTH / pixelSize);

			const float x = static_cast<float>(indexX) * pixelSizeInFloat + pixelSizeInFloat / 2.0f;
			const float y = static_cast<float>(indexY) * pixelSizeInFloat + pixelSizeInFloat / 2.0f;

			CSimpleSprite* texture = App::CreateSprite(RESOURCE_PATH("Opacity Texture.png"), 100, 1);
			texture->SetPosition(x, y);
			// TODO: Remove as alpha will be handled in raymarching
			texture->SetFrame(99);
			PIXELS[index] = std::unique_ptr<CSimpleSprite>(texture);
		}
	}

	void UpdatePixels(const std::vector<std::unique_ptr<CSimpleSprite>>& pixels,
	                  const std::vector<Vector4f>& colorBuffer)
	{
		if (pixels.size() != colorBuffer.size())
			throw std::invalid_argument("The sizes of pixels and colorBuffer must be the same.");

		const size_t threadCount = std::thread::hardware_concurrency();
		const size_t chunkSize = pixels.size() / threadCount;

		auto worker = [&](const size_t start, const size_t end)
		{
			for (size_t i = start; i < end; ++i)
			{
				const auto& color = colorBuffer[i];
				// const int alpha = static_cast<int>(color.w * 100 - 1);
				// pixels[i]->SetFrame(alpha);
				pixels[i]->SetColor(color.x, color.y, color.z);
			}
		};

		std::vector<std::thread> threads;

		for (size_t t = 0; t < threadCount; ++t)
		{
			size_t start = t * chunkSize;
			size_t end = (t == threadCount - 1) ? pixels.size() : start + chunkSize;
			threads.emplace_back(worker, start, end);
		}

		for (auto& thread : threads)
			thread.join();
	}

	float CalculateDepth(const float fov)
	{
		const float halfFov = fov / 2.0f;
		const float halfFovRadians = halfFov * (PI / 180.0f); // Convert degrees to radians
		return 1.0f / tan(halfFovRadians);
	}

	Vector4f RenderScene(const Vector3f& rayOrigin, const Vector3f& rayDirection)
	{
		Vector3f color = SKY_COLOR;

		auto [t, materialId] = IntersectScene(rayOrigin, rayDirection);
		const bool hasIntersected = t > 0.0f;

		if (hasIntersected)
		{
			const Vector3f hitPoint = rayOrigin + t * rayDirection;

			const bool isGround = materialId == 1.0f;

			// TODO: Using geom transform
			// Calculate Normal
			const Vector3f normal = isGround ? PLANE_NORMAL : CalculateSphereNormal(hitPoint, SPHERE_CENTER);

			if (isGround)
			{
				const float x = SdCheckerBoard(hitPoint);
				color = Vector3f{0.15f} + x * Vector3f{0.05f};
			}
			else color = Vector3f{0.0f, 1.0f, 0.0f};

			auto skyLight = ApplySkyLight(color, rayDirection, normal);
			float ambientOcclusion = CalculateAmbientOcclusion(hitPoint, normal);

			color = skyLight * ambientOcclusion;

			float fogFactor = ApplyFog(t, 1e-6f);

			color = color.lerp(SKY_COLOR, fogFactor);

			// Temporary fix for the object not casting a shadow
			// if (materialId != 1.0f)
			// {
			// 	// Apply Lighting
			// 	const Vector3f lightPosition = {-15, 15, 0};
			// 	const Vector3f light = ApplyLighting(hitPoint, normal, rayDirection, lightPosition);
			// 	const Vector3f shadow = ApplyShadow(hitPoint, lightPosition);
			//
			// 	// light * shadow
			// 	fragColor = light;
			// }
		}

		Vector4f fragColor = color;

		return fragColor;
	}

	std::tuple<float, float> IntersectScene(const Vector3f& rayOrigin, const Vector3f& rayDirection)
	{
		float t = -1.0f;
		float materialId = -1.0f;

		const float tFloor = -rayOrigin.dot(PLANE_NORMAL) / rayDirection.dot(PLANE_NORMAL);
		if (tFloor > 0.0f)
		{
			t = tFloor;
			materialId = PLANE_MATERIAL_ID;
		}

		// constexpr float sphereRadius = 1.0f;
		const float tSphere = IntersectSphere(rayOrigin, rayDirection, SPHERE_CENTER, SPHERE_RADIUS);

		if (tSphere > 0.0f && (t < 0.0f || tSphere < t))
		{
			t = tSphere;
			materialId = SPHERE_MATERIAL_ID;
		}

		return {t, materialId};
	}

	// Signed distance function for a sphere
	float SdSphere(const Vector3f& point, const float radius)
	{
		return point.magnitude() - radius;
	}

	float SdScene(const Vector3f& point)
	{
		// Plane
		float result = point.y;

		// Sphere
		result = Union(result, SdSphere(point - SPHERE_CENTER, SPHERE_RADIUS));

		return result;
	}

	float SdCheckerBoard(const Vector3f& point)
	{
		const int x = static_cast<int>(round(point.x + 0.5f));
		const int z = static_cast<int>(round(point.z + 0.5f));
		return static_cast<float>((x ^ z) & 1);
	}

	float Union(const float d1, const float d2)
	{
		return d1 < d2 ? d1 : d2;
	}

	float IntersectSphere(const Vector3f& rayOrigin, const Vector3f& rayDirection,
	                      const Vector3f& sphereCenter, const float sphereRadius)
	{
		float t = 0.0f;
		for (size_t i = 0; i < 100; i++)
		{
			// Iterate for a maximum of 100 steps
			Vector3f p = rayOrigin + rayDirection * t;
			const float d = SdSphere(p - sphereCenter, sphereRadius);
			if (d < 0.001f)
			{
				// If the distance is small enough, we have an intersection
				return t;
			}

			// Move along the ray by the distance
			t += d;
			if (t > 100.0)
			{
				// If we've moved too far, stop
				break;
			}
		}
		return -1.0f; // No intersection found
	}

	Vector3f CalculateSphereNormal(const Vector3f& hitPoint, const Vector3f& center)
	{
		return (hitPoint - center).normalize();
	}

	// TODO: Light Color, Material Color
	Vector4f ApplyLighting(const Vector3f& hitPoint, const Vector3f& normal,
	                       const Vector3f& rayDirection, const Vector3f& lightPosition)
	{
		// Direction from point to light
		Vector3f lightDir = (lightPosition - hitPoint).normalize();
		// Direction from point to camera (origin)
		Vector3f viewDir = -rayDirection.normalize();
		// Halfway vector between light and view
		Vector3f halfDir = (lightDir + viewDir).normalize();

		// Diffuse component
		float diffuse = max(normal.dot(lightDir), 0.0f);

		// Specular component
		constexpr float specularStrength = 0.5;
		constexpr float shininess = 32.0;
		const float spec = std::pow(max(normal.dot(halfDir), 0.0f), shininess);

		// White specular highlight
		Vector3f specular = Vector3f{1.0f} * (spec * specularStrength);

		// Ambient component
		Vector3f ambient{0.1f};

		Vector3f color = ambient + diffuse + specular;

		return {color.x, color.y, color.z, 1.0f};
	}

	Vector4f ApplyDirectionalLighting(const Vector3f& hitPoint, const Vector3f& normal)
	{
		return {};
	}

	Vector3f ApplySkyLight(const Vector3f& color, const Vector3f& rayDirection, const Vector3f& normal)
	{
		// Calculate diffuse lighting with an offset to brighten downward-facing surfaces,
		// simulating bounced light and avoiding complete darkness.
		constexpr float bias = 0.5f; // Offset to brighten downward-facing surfaces
		constexpr float scale = 0.5f; // Scale factor for normal.y contribution

		// Calculate diffuse lighting with clamping to ensure valid range [0, 1]
		const float diffuse = sqrt(std::clamp(bias + scale * normal.y, 0.0f, 1.0f));

		const Vector3f reflect = Reflect(rayDirection, normal);
		const float smoothSpecularFactor = SmoothStep(-0.2f, 0.2f, reflect.y);

		// schlick fresnel approximation
		constexpr float baseReflectivity = 0.04f;
		constexpr float oneMinusBaseReflectivity = 1.0f - baseReflectivity;
		float cosTheta = normal.dot(-rayDirection);
		cosTheta = std::clamp(cosTheta, 0.0f, 1.0f);
		const float fresnelReflectance = baseReflectivity + oneMinusBaseReflectivity * pow((1.0f - cosTheta), 5.0f);

		return color.hadamard(SKY_COLOR * diffuse) + smoothSpecularFactor * fresnelReflectance * SKY_COLOR;
	}

	float CalculateAmbientOcclusion(const Vector3f& hitPoint, const Vector3f& normal)
	{
		float value = 0.0f;
		float attenuationFactor = 1.0f;
		constexpr size_t maxIteration = 5;

		for (size_t i = 0; i < maxIteration; ++i)
		{
			const float minDistance = 0.025f * static_cast<float>(i);
			const Vector3f samplePoint = hitPoint + minDistance * normal;
			const auto distance = SdScene(samplePoint);

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

	float ApplyFog(const float t, const float density)
	{
		return 1.0f - exp(-density * t * t * t);
	}

	float SmoothStep(const float edge0, const float edge1, const float x)
	{
		const float t = std::clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
		return t * t * (3.0f - 2.0f * t);
	}

	Vector3f Reflect(const Vector3f& in, const Vector3f& normal)
	{
		return in - 2.0f * in.dot(normal) * normal;
	}
}
