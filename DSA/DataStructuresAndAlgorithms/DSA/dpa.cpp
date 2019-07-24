#include "dpa.h"
#include <stdexcept>
#include <algorithm>

size_t dpa::fastestRouteAssemblyLine(size_t e1, const std::vector<size_t>& L1, size_t x1, size_t e2, const std::vector<size_t>& L2, size_t x2, const std::vector<size_t>& t12, const std::vector<size_t>& t21)
{
	if (L1.size() == L2.size() &&
		t12.size() == t21.size())
	{
		std::vector<size_t> f1, f2;
		size_t n = L1.size();
		size_t result = UINT_MAX; // by default the time needed is infinite
		f1.push_back(e1 + L1[0]);
		f2.push_back(e2 + L2[0]);
		for (size_t i = 1; i < n; i++)
		{
			f1.push_back(std::min(f1[i - 1] + L1[i], f2[i - 1] + t21[i - 1] + L1[i]));
			f2.push_back(std::min(f2[i - 1] + L2[i], f1[i - 1] + t12[i - 1] + L2[i]));
		}
		result = std::min(f1[n - 1] + x1, f2[n - 1] + x2);
		return result;
	}
	throw std::invalid_argument("Invalid assembly lines");
}

std::vector<size_t> dpa::activitySelectionProblem(const vector<size_t>& s, const vector<size_t>& f)
{
	return vector<size_t>();
}

// lengths = 1, 2, 3, 4,  5,  6,  7,  8,  9, 10
// prices = [1, 5, 8, 9, 10, 17, 17, 20, 24, 30]
// r =   [0, 1, -*, ...         ...         -*]
// bottom-up dynamic algorithm implementation
std::vector<int> dpa::cutRodProblem(const vector<int>& prices)
{
	size_t n = prices.size();
	vector<int> r;
	r.reserve(n + 1);
	r.push_back(0);
	for (size_t i = 1; i <= n; ++i)
	{
		int q = INT_MIN; // -infinity
		for (size_t j = 0; j < i; ++j)
		{
			q = std::max(q, r[j] + prices[i - j - 1]);
		}
		r.push_back(q);
	}
	return r;
}

