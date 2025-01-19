#include "pch.h"
#include "Chromosome.h"
#include "Solver.h"
#include <random>
#include <string>
#include <unordered_set>

struct Solver::Impl
{
#pragma region API
	// Suggestion: Make it a multiple of 4 for better performance, 
	// as compilers can often auto-vectorize.
	size_t populationSize = 32;
	float crossoverRate = 0.75f;
	float mutationRate = 0.05f;
	size_t mutationCount = 1;
	size_t maxIterationTime = 64;

	Impl() : m_randomDevice(), m_generator(m_randomDevice())
	{
	}

	std::vector<size_t> Solve(const Vector3f& currentPosition, const std::shared_ptr<std::vector<Vector3f>>& nodes)
	{
		m_bestChromosome.reset();
		m_currentPosition = currentPosition;
		m_nodes = nodes;
		m_nodeCounts = nodes->size();

		calculate_distance();

		for (size_t i = 0; i < maxIterationTime; i++)
		{
			const Chromosome currentChromosome = iterate();
			if (currentChromosome.fitness > m_bestChromosome.fitness) m_bestChromosome = currentChromosome;
		}

		return std::move(m_bestChromosome.stops);
	}

#pragma endregion

private:
#pragma region Internal
	std::shared_ptr<std::vector<Vector3f>> m_nodes;
	size_t m_nodeCounts = 0;

	std::vector<float> m_distances;

	Vector3f m_currentPosition;
	Chromosome m_bestChromosome;

	std::random_device m_randomDevice;
	std::mt19937 m_generator;

	Chromosome iterate()
	{
		auto currentPopulation = generate_population();
		for (size_t i = 0; i < maxIterationTime; i++)
		{
			std::vector<Chromosome> newPopulation;
			newPopulation.reserve(currentPopulation.size());

			for (size_t j = 0; j < populationSize; j++)
			{
				const size_t parentAIndex = select_parent(currentPopulation);
				const Chromosome& parentA = currentPopulation[parentAIndex];
				Chromosome child = parentA;

				std::uniform_real_distribution dist(0.0f, 1.0f);
				const bool doCrossOver = dist(m_generator) < crossoverRate;
				if (doCrossOver)
				{
					const size_t parentBIndex = select_parent(currentPopulation);
					const Chromosome& parentB = currentPopulation[parentBIndex];
					child = cross_over(parentA, parentB);
				}

				Mutate(child);

				const float newFitness = calculate_fitness(child.stops);
				child.fitness = newFitness;
				newPopulation.push_back(std::move(child));
			}

			const auto& bestChromosome = currentPopulation[find_best_chromosome(currentPopulation)];
			newPopulation[0] = bestChromosome;
			currentPopulation = newPopulation;
		}

		return std::move(currentPopulation[find_best_chromosome(currentPopulation)]);
	}

	void calculate_distance()
	{
		const std::vector<Vector3f>& nodes = *m_nodes;

		m_distances.reserve(m_nodeCounts * (m_nodeCounts - 1) / 2);

		size_t index = 0;
		for (size_t i = 0; i < m_nodeCounts; i++)
		{
			for (size_t j = i + 1; j < m_nodeCounts; j++)
			{
				// Get positions of the two m_nodes
				const Vector3f& self = nodes[i];
				const Vector3f& other = nodes[j];

				// Calculate distance
				const float distance = self.distance(other);

				// Store distance in the array
				m_distances[index] = distance;
				++index;
			}
		}
	}

	[[nodiscard]] float get_distance(size_t i, size_t j, const size_t nodeCounts) const
	{
		if (i == j) return 0.0f;

		if (i > j) std::swap(i, j);

		const size_t totalDistances = nodeCounts * (nodeCounts - 1) / 2;
		const size_t index = i * (2 * nodeCounts - i - 1) / 2 + (j - i - 1);

		if (index >= totalDistances) throw std::out_of_range("Invalid distance index: " + std::to_string(index));

		return m_distances[index];
	}

	[[nodiscard]] std::vector<Chromosome> generate_population() const
	{
		auto population = std::vector<Chromosome>(populationSize);
		for (auto& i : population) i = generate_chromosome();
		return population;
	}

	[[nodiscard]] Chromosome generate_chromosome() const
	{
		auto stops = std::vector<size_t>(m_nodeCounts);
		for (size_t i = 0; i < m_nodeCounts; i++)
			stops[i] = i;

		shuffle(stops);
		float fitness = calculate_fitness(stops);

		return {std::move(stops), fitness};
	}

	[[nodiscard]] float calculate_fitness(const std::vector<size_t>& stops) const
	{
		const std::vector<Vector3f>& nodes = *m_nodes;
		const auto& firstNode = nodes[stops[0]];
		float distance = m_currentPosition.distance(firstNode);

		for (size_t i = 1; i < m_nodeCounts; i++) distance += get_distance(stops[i - 1], stops[i], m_nodeCounts);

		return 1 / distance;
	}

	static void shuffle(std::vector<size_t>& array)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::shuffle(array.begin(), array.end(), gen);
	}

	// roulette wheel selection
	size_t select_parent(const std::vector<Chromosome>& population)
	{
		float totalFitness = 0;

		for (auto& chromosome : population) totalFitness += chromosome.fitness;

		// Generate a random number between 0 and totalFitness
		std::uniform_real_distribution dist(0.0f, totalFitness);
		const float randomPoint = dist(m_generator);

		// Iterate through the population to find the selected chromosome
		float cumulativeFitness = 0;
		for (size_t i = 0; i < population.size(); i++)
		{
			auto& chromosome = population[i];
			cumulativeFitness += chromosome.fitness;
			// If the cumulative fitness surpasses the random point, select this chromosome
			if (cumulativeFitness >= randomPoint) return i;
		}

		// In case no chromosome is selected (edge case), return the last one
		return population.size() - 1;
	}

	// Performs Partially Mapped Crossover between two parent chromosomes to generate a child chromosome.
	Chromosome cross_over(const Chromosome& parentA, const Chromosome& parentB)
	{
		size_t crossOverPointStart;
		size_t crossOverPointEnd;

		do
		{
			std::uniform_int_distribution<size_t> dist(0, m_nodeCounts - 1);
			crossOverPointStart = dist(m_generator);
			crossOverPointEnd = dist(m_generator);
		}
		while (crossOverPointStart == crossOverPointEnd);

		if (crossOverPointStart > crossOverPointEnd) std::swap(crossOverPointStart, crossOverPointEnd);

		std::vector<size_t> childStops(m_nodeCounts);
		std::fill(childStops.begin(), childStops.end(), std::numeric_limits<size_t>::max());

		std::unordered_set<size_t> filledStops;
		for (size_t i = crossOverPointStart; i < crossOverPointEnd; i++)
		{
			childStops[i] = parentA.stops[i];
			filledStops.insert(parentA.stops[i]);
		}

		size_t parentBIndex = 0;

		for (size_t i = 0; i < m_nodeCounts; i++)
		{
			if (childStops[i] != std::numeric_limits<size_t>::max()) continue;

			while (filledStops.find(parentB.stops[parentBIndex]) != filledStops.end())
				parentBIndex++;

			childStops[i] = parentB.stops[parentBIndex];
			filledStops.insert(parentB.stops[parentBIndex]);
		}

		return Chromosome{std::move(childStops)};
	}

	// Randomly swaps two distinct indices in the chromosome stops array mutationCount times based on mutationRate.
	void Mutate(Chromosome& chromosome)
	{
		for (size_t i = 0; i < mutationCount; i++)
		{
			std::uniform_real_distribution mutationChanceDist(0.0f, 1.0f);
			const bool shouldMutate = mutationChanceDist(m_generator) < mutationRate;
			if (!shouldMutate) continue;

			std::uniform_int_distribution<size_t> indexDist(0, chromosome.stops.size() - 1);
			size_t startIndex, endIndex;

			do
			{
				startIndex = indexDist(m_generator);
				endIndex = indexDist(m_generator);
			}
			while (startIndex == endIndex);

			std::swap(chromosome.stops[startIndex], chromosome.stops[endIndex]);
		}
	}

	static size_t find_best_chromosome(const std::vector<Chromosome>& population)
	{
		float maxFitness = std::numeric_limits<float>::lowest();
		size_t bestChromosomeIndex = 0;

		for (size_t i = 0; i < population.size(); ++i)
		{
			if (population[i].fitness > maxFitness)
			{
				maxFitness = population[i].fitness;
				bestChromosomeIndex = i;
			}
		}

		return bestChromosomeIndex;
	}
#pragma endregion
};


Solver::Solver()
{
	pImpl = new Impl();
}

Solver::~Solver()
{
	delete pImpl;
}

Solver::Solver(Solver&& other) noexcept: pImpl(other.pImpl)
{
}

Solver& Solver::operator=(const Solver& other)
{
	if (this == &other) return *this;
	pImpl = other.pImpl;
	return *this;
}

Solver& Solver::operator=(Solver&& other) noexcept
{
	if (this == &other) return *this;
	pImpl = other.pImpl;
	return *this;
}

std::vector<size_t> Solver::Solve(const Vector3f& currentPosition, const Nodes& nodes) const
{
	return pImpl->Solve(currentPosition, nodes);
}
