#include "pch.h"
#include "Chromosome.h"

Chromosome::Chromosome(): fitness(-1.0f)
{
}

Chromosome::~Chromosome() = default;

Chromosome::Chromosome(std::vector<size_t>&& stops): fitness(-1)
{
	this->stops = std::move(stops);
}

Chromosome::Chromosome(std::vector<size_t>&& stops, const float fitness): stops(std::move(stops)), fitness(fitness)
{
}

Chromosome::Chromosome(const Chromosome& other) = default;

Chromosome::Chromosome(Chromosome&& other) noexcept: stops(std::move(other.stops)),
                                                     fitness(other.fitness)
{
}

Chromosome& Chromosome::operator=(const Chromosome& other)
{
	if (this == &other) return *this;
	stops = other.stops;
	fitness = other.fitness;
	return *this;
}

Chromosome& Chromosome::operator=(Chromosome&& other) noexcept
{
	if (this == &other) return *this;
	stops = std::move(other.stops);
	fitness = other.fitness;
	return *this;
}

void Chromosome::reset()
{
	stops.clear();
	fitness = -1.0f;
}
