#pragma once
#include <array>
#include <ostream>
#include <vector>

namespace dsa
{
	class Matrix
	{
	public:
		Matrix(size_t m, size_t n);

		const std::vector<std::vector<double>>& Raw() const;
		
		size_t M() const;

		size_t N() const;

		Matrix operator*(const Matrix& other) const;

		double& operator()(size_t i, size_t j);

		double operator()(size_t i, size_t j) const;

	private:
		size_t m, n;
		std::vector<std::vector<double>> matrix;
	};

	std::ostream& operator<<(std::ostream& out, const Matrix& m);

	Matrix calcOptimalParanthesiz(const std::vector<size_t>& dimensions);
}

