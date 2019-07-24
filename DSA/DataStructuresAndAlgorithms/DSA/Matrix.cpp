#include "Matrix.h"
#include <memory>
#include <stdexcept>
dsa::Matrix::Matrix(size_t m, size_t n)
	:m(m),n(n)
{
	this->matrix.reserve(m);
	std::vector<double> a;
	for (size_t i = 0; i < m; i++)
	{
		this->matrix.push_back(a);
	}

	for (size_t i = 0; i < m; i++)
	{
		this->matrix[i].reserve(n);
		this->matrix[i].assign(n, 0.0);
	}
}

const std::vector<std::vector<double>>& dsa::Matrix::Raw() const
{
	return this->matrix;
}

size_t dsa::Matrix::M() const
{
	return m;
}

size_t dsa::Matrix::N() const
{
	return n;
}

dsa::Matrix dsa::Matrix::operator*(const Matrix& other) const
{
	if (this->N() == other.M())
	{
		Matrix result(this->M(), other.N());

		for (size_t i = 0; i < result.M(); i++)
		{
			for (size_t j = 0; j < result.N(); j++)
			{
				double c = 0.0;
				for (size_t k = 0; k < this->N(); k++)
				{
					c += ((*this)(i, k) * other(k, j));
				}
				result(i, j) = c;
			}
		}

		return result;
	}

	throw std::exception("Invalid Matrix multiplication arguments");
}

double& dsa::Matrix::operator()(size_t i, size_t j)
{
	if (i < m && j < n)
	{
		return this->matrix[i][j];
	}
	throw std::out_of_range("Index out of range");
}

double dsa::Matrix::operator()(size_t i, size_t j) const
{
	return this->matrix[i][j];
}

std::ostream& dsa::operator<<(std::ostream& out, const Matrix& m)
{
	for (size_t i = 0; i < m.M(); ++i)
	{
		for (size_t j = 0; j < m.N(); ++j)
		{
			out << m.Raw()[i][j] << ' ';
		}
		out << std::endl;
	}
	return out;
}

dsa::Matrix dsa::calcOptimalParanthesiz(const std::vector<size_t>& p)
{
	size_t n = p.size() - 1;
	Matrix ms(n, n);
	for (size_t l = 1; l < n; l++) // the chain length
	{
		for (size_t i = 0; i < n - l; i++)
		{
			size_t j = i + l - 1;
			ms(i, j) = UINT_MAX;
			for (size_t k = i; k < j - 1; k++)
			{
				size_t q = ms(i, k) + ms(k + 1, j) + p[i - 1] * p[k] * p[j];
				if (q < ms(i, j))
				{
					ms(i, j) = q;
					ms(j, i) = k;
				}
			}
		}
	}

	return ms;
}
