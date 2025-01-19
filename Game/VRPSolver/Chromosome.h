#pragma once
#include <vector>

struct Chromosome final
{
	std::vector<size_t> stops;
	float fitness;

#pragma region The Big Fives
	Chromosome();
	~Chromosome();
	explicit Chromosome(std::vector<size_t>&& stops);
	Chromosome(std::vector<size_t>&& stops, float fitness);
	Chromosome(const Chromosome& other);
	Chromosome(Chromosome&& other) noexcept;
	Chromosome& operator=(const Chromosome& other);
	Chromosome& operator=(Chromosome&& other) noexcept;
#pragma endregion

	void reset();
};
