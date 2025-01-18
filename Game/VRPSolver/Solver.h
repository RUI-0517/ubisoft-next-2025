#pragma once
#include "VRPSolver.h"

class VRPSOLVER_API Solver
{
	struct Impl;
	Impl* pImpl;

public:
	Solver();
	~Solver();

	char helloSolver() const;
};
