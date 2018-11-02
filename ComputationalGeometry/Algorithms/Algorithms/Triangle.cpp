#include "Triangle.h"



Triangle::Triangle()
{
}

Triangle::Triangle(const vec2d<double>& A, const vec2d<double>& B, const vec2d<double>& C)
{
	this->A = A;
	this->B = B;
	this->C = C;
}


Triangle::~Triangle()
{
}

bool Triangle::operator()(const vec2d<double>& p)
{
	return false;
}

double Triangle::OrientedArea(const vec2d<double>& A, const vec2d<double>& B, const vec2d<double>& C)
{
	return ((B - A) ^ (C - A)) / 2.0;
}

double Triangle::OrientedArea() const
{
	return Triangle::OrientedArea(this->A, this->B, this->C);
}

double Triangle::Area() const
{
	double oriented = this->OrientedArea();
	if (!oriented)
	{
		return -1.0 * oriented;
	}
	return oriented;
	return 0.0;
}
