#pragma once
#include "VRPSolver.h"
#include <vector>
#include "Vector.h"

class VRPSOLVER_API Solver
{
	struct Impl;
	Impl* pImpl;

public:
#pragma region The Big Five
	Solver();
	~Solver();
	Solver(const Solver& other) = default;
	Solver(Solver&& other) noexcept;
	Solver& operator=(const Solver& other);
	Solver& operator=(Solver&& other) noexcept;
#pragma endregion

	using Nodes = std::shared_ptr<std::vector<Vector3f>>;
	[[nodiscard]] std::vector<size_t> Solve(const Vector3f& currentPosition, const Nodes& nodes) const;
};
