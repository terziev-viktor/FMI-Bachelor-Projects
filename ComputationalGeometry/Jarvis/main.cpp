#include "vecta.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
using std::sort;
using std::vector;
using std::ifstream;
using std::cout;
using std::cin;
using std::endl;
using namespace vecta;

vector<vec2d<double>> points;

void read_points()
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

std::ostream & operator<<(std::ostream & out, const vector<vec2d<double>> & obj)
{
	for (size_t i = 0; i < obj.size(); ++i)
	{
		out << '(' << obj[i].x << ',' << obj[i].y << ')' << endl;
	}
	return out;
}

void Jarvis_Demo()
{
	// wiki
	vector<vec2d<double>> hull;
	vec2d<double> pointOnHull = *std::max_element(points.begin(), points.end(), [](const vec2d<double> &a, const vec2d<double> &b) -> bool {
		if (a.x == b.x)
		{
			return a.y > b.y;
		}
		return a.x > b.x;
	});
	vec2d<double> nextOnHull = pointOnHull;

	do
	{	
		hull.push_back(nextOnHull);
		pointOnHull = nextOnHull;

		nextOnHull = points[0];
		for (size_t i = 1; i < points.size(); ++i)
		{
			if (nextOnHull == pointOnHull || ((nextOnHull - pointOnHull) ^ (points[i] - pointOnHull)) < 0.0)
			{
				nextOnHull = points[i];
			}
		}

	} while (nextOnHull != hull[0]);

	cout << "HULL:" << endl;
	cout << hull << endl;
}

int main()
{
	read_points();
	Jarvis_Demo();

	return 0;
}