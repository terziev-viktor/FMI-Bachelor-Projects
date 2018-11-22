#include <vector>
#include <fstream>
#include <iostream>
#include <stack>
#include <algorithm>
#include "../Algorithms/vecta.h"

using namespace vecta;
using namespace std;

typedef vector<vec2d<double>> PointsSet;

class CoolStack : public stack<vec2d<double>>
{
public:
	vec2d<double> next_to_top() const
	{
		return this->c[this->size() - 2];
	}
};

std::ostream & operator<<(std::ostream & out, const PointsSet & obj)
{
	for (size_t i = 0; i < obj.size(); ++i)
	{
		out << '(' << obj[i].x << ',' << obj[i].y << ')' << endl;
	}
	return out;
}

std::ostream & operator<<(std::ostream & out, const vec2d<double> & obj)
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

bool ccw(const vec2d<double> & A, const vec2d<double> & B, const vec2d<double> & C)
{
	return ((B - A) ^ (C - A)) > 0.0;
}

int main()
{
	PointsSet points;
	read_points(points); // input
	
	// finding the first element of the hull
	PointsSet::iterator B = max_element(points.begin(), points.end(), [](const vec2d<double> & a, const vec2d<double> & b) -> bool {
		if (a.y == b.y)
		{
			return a.x > b.x;
		}
		return a.y > b.y;
	});

	// swapping the first element of the set with the first in the hull 
	swap(points[0], points[distance(points.begin(), B)]);
	
	// sorting the elements by angle with respect to the first element
	sort(points.begin(), points.end(), [&points](const vec2d<double> & b, const vec2d<double> & c) -> bool {
		return ((b - points[0]) ^ (c - points[0])) > 0.0;
	});

	cout << points << endl;
	CoolStack hull;
	hull.push(points[0]);
	hull.push(points[1]);
	hull.push(points[2]);
	for (size_t i = 3; i < points.size(); ++i)
	{
		while (!ccw(hull.next_to_top(), hull.top(), points[i]))
		{
			hull.pop();
		}
		hull.push(points[i]);
	}

	while(hull.empty() == false)
	{
		cout << hull.top() << endl;
		hull.pop();
	}

	return 0;
}