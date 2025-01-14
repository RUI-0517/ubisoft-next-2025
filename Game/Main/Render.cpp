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
		const Vector4f skyColor = {0.0f, 0.0f, 0.0f, 1.0f};

		Vector4f fragColor = skyColor;

		auto [t, materialId] = IntersectScene(rayOrigin, rayDirection);
		const bool hasIntersected = t > 0.0f;

		if (hasIntersected)
		{
			const Vector3f hitPoint = rayOrigin + t * rayDirection;

			// TODO: Using geom transform
			// Calculate Normal
			const Vector3f normal =
				materialId == 1.0f ? PLANE_NORMAL : CalculateSphereNormal(hitPoint, SPHERE_CENTER);

			// Apply Lighting
			const Vector3f lightPosition = {-15, 15, 0};
			fragColor = ApplyLighting(hitPoint, normal, rayDirection, lightPosition);
		}

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
	float SdSphere(const Vector3f& p, const float s)
	{
		return p.magnitude() - s;
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
}
