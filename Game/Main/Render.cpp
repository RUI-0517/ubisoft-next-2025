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
				const int alpha = static_cast<int>(color.w * 100 - 1);
				pixels[i]->SetFrame(alpha);
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

	Vector3f CalculateRayDirection(const float u, const float v, const float depth)
	{
		return Vector3f(u, v, depth).normalize();
	}

	// Signed distance function for a sphere
	float SdSphere(const Vector3f& p, const float s)
	{
		return p.magnitude() - s;
	}

	float IntersectSphere(const Vector3f& rayOrigin, const Vector3f& rayDirection,
	                      const Vector3f& sphereCenter, const float sphereRadius)
	{
		float t = 0.0;
		for (int i = 0; i < 100; i++)
		{
			// Iterate for a maximum of 100 steps
			Vector3f p = rayOrigin + rayDirection * t;
			const float d = SdSphere(p - sphereCenter, sphereRadius);
			if (d < 0.001)
			{
				// If the distance is small enough, we have an intersection
				return t;
			}
			t += d; // Move along the ray by the distance
			if (t > 100.0)
			{
				// If we've moved too far, stop
				break;
			}
		}
		return -1.0; // No intersection found
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
		Vector3f specular = Vector3f(1.0f) * (spec * specularStrength);

		// Ambient component
		Vector3f ambient(0.1f);

		Vector3f color = ambient + diffuse + specular;

		return {color.x, color.y, color.z, 1.0f};
	}

	// Apply ACES tone mapping
	Vector4f AcesFilm(const Vector4f& color)
	{
		constexpr float a = 2.51f;
		constexpr float b = 0.03f;
		constexpr float c = 2.43f;
		constexpr float d = 0.59f;
		constexpr float e = 0.14f;

		// TODO: SSE Version
		Vector3f result;
		for (int i = 0; i < 3; ++i)
		{
			const float numerator = color[i] * (color[i] * a + b);
			const float denominator = color[i] * (color[i] * c + d) + e;
			result[i] = std::clamp(numerator / denominator, 0.0f, 1.0f);
		}
		return {result.x, result.y, result.z, 1.0f};
	}
}
