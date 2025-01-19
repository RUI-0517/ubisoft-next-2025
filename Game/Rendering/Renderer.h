#pragma once
#include <functional>
#include "Vector.h"
#include "Rendering.h"

class RENDERING_API Renderer
{
	struct Impl;
	Impl* pImpl;

	/**
	 * @brief A callable type representing a task to be executed in parallel on a portion of a buffer.
	 * 
	 * @param buffer The `std::vector<Vector4f>` to process.
	 * @param start The starting index (inclusive) of the range to process.
	 * @param end The ending index (exclusive) of the range to process.
	 */
	using ParallelTask = std::function<void(std::vector<Vector4f>&, size_t)>;
	using ParallelTask2D = std::function<void(std::vector<Vector4f>&, size_t, float, float)>;

public:
#pragma region The Big Fives
	Renderer(size_t width, size_t height);
	virtual ~Renderer();

	Renderer(const Renderer& other);
	Renderer(Renderer&& other) noexcept;
	Renderer& operator=(const Renderer& other);
	Renderer& operator=(Renderer&& other) noexcept;
#pragma endregion

#pragma region APIs
	[[nodiscard]] size_t getWidth() const;
	[[nodiscard]] size_t getHeight() const;

	void setPixel(size_t x, size_t y, const Vector4f& color) const;
	[[nodiscard]] const Vector4f& getPixel(size_t x, size_t y) const;
	Vector4f& getPixel(size_t x, size_t y);

	[[nodiscard]] const std::vector<Vector4f>& getBuffer() const;
	std::vector<Vector4f>& getBuffer();
	void clearBuffer() const;

	void setClearColor(const Vector4f& clearColor) const;

	void each(size_t threadCount, const ParallelTask& task) const;
	void each(size_t threadCount, const ParallelTask2D& task) const;
#pragma endregion
};
