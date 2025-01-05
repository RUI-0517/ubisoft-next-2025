#include "pch.h"
#include "Renderer.h"
#include <thread>

struct Renderer::Impl
{
	std::vector<Vector4f> buffer;
	size_t width;
	size_t height;
	Vector4f clearColor;

#pragma region The Big Fives
	Impl(const size_t width, const size_t height) : buffer(width * height, Vector4f{}), width(width), height(height)
	{
	}

	~Impl() = default;
	Impl(const Impl& other) = default;

	Impl(Impl&& other) noexcept : buffer(std::move(other.buffer)),
	                              width(other.width), height(other.height), clearColor(other.clearColor)
	{
	}

	Impl& operator=(const Impl& other)
	{
		if (this == &other) return *this;
		buffer = other.buffer;
		width = other.width;
		height = other.height;
		clearColor = other.clearColor;
		return *this;
	}

	Impl& operator=(Impl&& other) noexcept
	{
		if (this == &other) return *this;
		buffer = std::move(other.buffer);
		width = other.width;
		height = other.height;
		clearColor = other.clearColor;
		return *this;
	}
#pragma endregion

	/**
	 * @brief Executes a given task on the buffer by dividing it into chunks and processing them in parallel.
	 * 
	 * @note
	 * - This function is not thread-safe with respect to concurrent calls to `each`.
	 * - Avoid calling this function from multiple threads if the task modifies overlapping data in `buffer`.
	 * 
	 * @param threadCount The number of threads to use for parallel processing.
	 * @param task A callable task (of type `ParallelTask`) that operates on each chunk of the buffer.
	 */
	void each(const size_t threadCount, const ParallelTask& task)
	{
		if (threadCount == 0)
			throw std::invalid_argument("Thread count must be greater than 0.");
		if (threadCount > buffer.size())
			throw std::invalid_argument("Thread count must not exceed the size of the buffer.");

		const size_t chunkSize = buffer.size() / threadCount;

		std::vector<std::thread> threads;

		auto threadTask = [&](const size_t startIndex, const size_t endIndex)
		{
			for (size_t index = startIndex; index < endIndex; ++index)
				task(buffer, index);
		};

		for (size_t i = 0; i < threadCount; ++i)
		{
			const size_t startIndex = i * chunkSize;
			const size_t endIndex = (i == threadCount - 1) ? buffer.size() : startIndex + chunkSize;
			threads.emplace_back(threadTask, startIndex, endIndex);
		}

		for (auto& thread : threads)
			thread.join();
	}

	void each(const size_t threadCount, const ParallelTask2D& task)
	{
		if (threadCount == 0)
			throw std::invalid_argument("Thread count must be greater than 0.");
		if (threadCount > buffer.size())
			throw std::invalid_argument("Thread count must not exceed the size of the buffer.");

		const size_t chunkSize = buffer.size() / threadCount;

		std::vector<std::thread> threads;

		auto threadTask = [&](const size_t startIndex, const size_t endIndex)
		{
			for (size_t index = startIndex; index < endIndex; ++index)
			{
				const size_t x = index % width;
				const size_t y = index / width;
				const float u = static_cast<float>(x) / static_cast<float>(width - 1);
				const float v = static_cast<float>(y) / static_cast<float>(height - 1);
				task(buffer, index, u, v);
			}
		};

		for (size_t i = 0; i < threadCount; ++i)
		{
			const size_t startIndex = i * chunkSize;
			const size_t endIndex = (i == threadCount - 1) ? buffer.size() : startIndex + chunkSize;
			threads.emplace_back(threadTask, startIndex, endIndex);
		}

		for (auto& thread : threads)
			thread.join();
	}
};

Renderer::Renderer(const size_t width, const size_t height)
{
	pImpl = new Impl(width, height);
}

Renderer::~Renderer()
{
	delete pImpl;
}

Renderer::Renderer(const Renderer& other) = default;

Renderer::Renderer(Renderer&& other) noexcept : pImpl(other.pImpl)
{
}

Renderer& Renderer::operator=(const Renderer& other)
{
	if (this == &other)
		return *this;
	pImpl = other.pImpl;
	return *this;
}

Renderer& Renderer::operator=(Renderer&& other) noexcept
{
	if (this == &other)
		return *this;
	pImpl = other.pImpl;
	return *this;
}

size_t Renderer::getWidth() const
{
	return pImpl->width;
}

size_t Renderer::getHeight() const
{
	return pImpl->height;
}

void Renderer::setPixel(const size_t x, const size_t y, const Vector4f& color) const
{
	if (x < pImpl->width && y < pImpl->height)
		(pImpl->buffer)[y * pImpl->width + x] = color;
}

const Vector4f& Renderer::getPixel(const size_t x, const size_t y) const
{
	return (pImpl->buffer)[y * pImpl->width + x];
}

Vector4f& Renderer::getPixel(const size_t x, const size_t y)
{
	return (pImpl->buffer)[y * pImpl->width + x];
}

const std::vector<Vector4f>& Renderer::getBuffer() const
{
	return pImpl->buffer;
}

std::vector<Vector4f>& Renderer::getBuffer()
{
	return pImpl->buffer;
}

void Renderer::clearBuffer() const
{
	std::fill(pImpl->buffer.begin(), pImpl->buffer.end(), pImpl->clearColor);
}

void Renderer::setClearColor(const Vector4f& clearColor) const
{
	pImpl->clearColor = clearColor;
}

void Renderer::each(const size_t threadCount, const ParallelTask& task) const
{
	pImpl->each(threadCount, task);
}

void Renderer::each(const size_t threadCount, const ParallelTask2D& task) const
{
	pImpl->each(threadCount, task);
}
