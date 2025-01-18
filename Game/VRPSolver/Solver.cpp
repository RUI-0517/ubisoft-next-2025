#include "pch.h"
#include "Solver.h"

struct Solver::Impl
{
	char c = 'h';

	Impl() = default;

	char helloSolver() const
	{
		return c;
	}
};

Solver::Solver()
{
	pImpl = new Impl();
}

Solver::~Solver()
{
	delete pImpl;
}

char Solver::helloSolver() const
{
	return pImpl->helloSolver();
}
