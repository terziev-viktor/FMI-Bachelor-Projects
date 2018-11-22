#pragma once
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include "../Algorithms/vecta.h"
#include "../Algorithms/cool_stack.hpp"
using namespace vecta;
using namespace std;
typedef vec2d<double> Point;
typedef vector<Point> PointsSet;

std::ostream & operator<<(std::ostream & out, const PointsSet & obj)
{
	for (size_t i = 0; i < obj.size(); ++i)
	{
		out << '(' << obj[i].x << ',' << obj[i].y << ')' << endl;
	}
	return out;
}

std::ostream & operator<<(std::ostream & out, const Point & obj)
{
	out << '(' << obj.x << ',' << obj.y << ')';
	return out;
}

void read_points(PointsSet & points)
{
	ifstream in("points.txt");
	vec2d<double> p;
	while (in)
	{
		in >> p.x;
		in >> p.y;
		if (in.eof())
		{
			points.push_back(p);
			break;
		}
		points.push_back(p);
	}
	in.close();
}

double ccw(const Point & A, const Point & B, const Point & C)
{
	return ((B - A) ^ (C - A));
}


