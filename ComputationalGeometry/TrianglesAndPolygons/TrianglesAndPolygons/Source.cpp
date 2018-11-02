#include <iostream>
#include <vector>
#include "vecta.h"
using namespace std;
using namespace vecta;
using std::vector;

class Triangle
{
public:
	vec2d<double> A, B, C;
public:
	Triangle() {}

	Triangle(const vec2d<double> & A, const vec2d<double> & B, const vec2d<double> & C) 
		:A(A),B(B),C(C)
	{
		
	}

	// Returns whether Point p is in the triangle or not
	bool operator()(const vec2d<double> & P)
	{
		return Triangle::PointIsIn(A, B, C, P);
	}

	static bool PointIsIn(const vec2d<double> & A, const vec2d<double> & B, const vec2d<double> & C, const vec2d<double> & P)
	{
		double a = Triangle::OrientedArea(P, B, C) / Triangle::OrientedArea(A, B, C);
		double b = Triangle::OrientedArea(A, P, C) / Triangle::OrientedArea(A, B, C);

		return a >= 0.0 && a <= 1.0 && b >= 0.0 && b <= 1.0;
	}

	static double OrientedArea(const vec2d<double> & A, const vec2d<double> & B, const vec2d<double> & C)
	{
		return ((B - A) ^ (C - A)) / 2;
	}

	double OrientedArea() const
	{
		return Triangle::OrientedArea(this->A, this->B, this->C);
	}

	double Area() const
	{
		double area = this->OrientedArea();
		if (!area)
		{
			return -area;
		}
		return area;
	}
};

class Polygon
{
public:
	vector<vec2d<double>> points;
	
	static double OrientedArea(const vector<vec2d<double>> & points)
	{
		double result = 0.0;
		for (size_t i = 0; i < points.size() - 1; ++i)
		{
			result += points[i] ^ points[i + 1];
		}
		return result / 2.0;
	}

	bool operator()(const vec2d<double> & p)
	{
		int c = 0;
		for (size_t i = 0; i < this->points.size() - 1; ++i)
		{
			// check if p is part of the polygon
			if (p == points[i])
				return true;
			
			// Check if we are on the currect vertex
			if (points[i].y == p.y && points[i + 1].y == p.y && (p.x - points[i].x) * (p.x - points[i + 1].x) < 0)
				return true;
			
			// check if we slide on the vertex and skip if so
			if (points[i].y == p.y && points[i + 1].y == p.y)
				continue;
			
			if (p.y > points[i].y && p.y < points[i+1].y && Triangle::OrientedArea(points[i], points[i + 1], p) > 0)
				++c;
			
			if (p.y < points[i].y && p.y > points[i+1].y && Triangle::OrientedArea(points[i], points[i + 1], p) < 0)
				++c;
			
			// check if we hit the top of the vertex, count if so and dont count otherwise
			if (p.y == points[i + 1].y)
				++c;
			
		}
		return c % 2 == 1;
	}
};

int main()
{
	Polygon poly;
	poly.points.push_back(vec2d<double>(0.0, 0.0));
	poly.points.push_back(vec2d<double>(5.0, 0.0));
	poly.points.push_back(vec2d<double>(5.0, 5.0));
	poly.points.push_back(vec2d<double>(0.0, 5.0));
	poly.points.push_back(vec2d<double>(0.0, 0.0));

	double x, y;
	while (true)
	{
		cin >> x >> y;
		cout << (poly(vec2d<double>(x, y)) ? "true":"false") << endl;
	}
	
	return 0;
}