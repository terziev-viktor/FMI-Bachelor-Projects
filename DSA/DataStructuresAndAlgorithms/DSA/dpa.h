#pragma once
#include <memory>
#include <vector>
// Dynamic Programming Algorithms
namespace dpa
{
	using std::vector;

	size_t fastestRouteAssemblyLine(size_t e1, const std::vector<size_t> & L1, size_t x1, size_t e2, const std::vector<size_t> & L2, size_t x2,
		const std::vector<size_t> & t12, const std::vector<size_t> & t21);

	vector<size_t> activitySelectionProblem(const vector<size_t>& s, const vector<size_t>& f);

	vector<int> cutRodProblem(const vector<int>& prices);
}