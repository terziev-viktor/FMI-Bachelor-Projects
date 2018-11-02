#include <iostream>
#include <algorithm>
#include <vector>
#include <stack>
#include <fstream>
#include "cool_stack.hpp"

using namespace std;

class Point
{
private:
	double x, y;
public:
	Point() { x = 0.0; y = 0.0; }

	Point(double x, double y)
	{
		this->x = x;
		this->y = y;
	}

	double & X()
	{
		return this->x;
	}

	double X() const
	{
		return this->x;
	}

	double & Y()
	{
		return this->y;
	}

	double Y() const
	{
		return this->y;
	}
};

ostream & operator<<(ostream & os, const Point & obj)
{
	os << '(' << obj.X() << ',' << obj.Y() << ')';
	return os;
}

void read_points(vector<Point> & points)
{
	ifstream in("points.txt");
	Point p;
	while (in)
	{
		in >> p.X();
		in >> p.Y();
		if (in.eof())
		{
			points.push_back(p);
			break;
		}
		points.push_back(p);
	}
	in.close();
}

void print(const vector<Point> & points)
{
	for (size_t i = 0; i < points.size(); ++i)
	{
		cout << points[i] << endl;
	}
}

double ccw(const Point & a, const Point & b, const Point & c)
{
	return (b.X() - a.X()) * (c.Y() - a.Y()) - (b.Y() - a.Y()) * (c.X() - a.X());
}

void swap_min_with_first(vector<Point> & points)
{
	size_t min = 0;
	for (size_t i = 1; i < points.size(); ++i)
	{
		if ((points[min].Y() == points[i].Y())
			&& points[min].X() > points[i].X())
		{
			min = i;
		}
		else if (points[min].Y() > points[i].Y())
		{
			min = i;
		}
	}
	Point first = points[0];
	points[0] = points[min];
	points[min] = first;
}

void Graham_demo()
{
	vector<Point> points;
	read_points(points);
	swap_min_with_first(points);
	sort(points.begin(), points.end(), [&](const Point & a, const Point & b) -> bool {
		return ccw(points[0], a, b) > 0.0;
	});
	print(points);
	cool_stack<Point> hull;
	hull.push(points[0]);
	hull.push(points[1]);
	hull.push(points[2]);
	for (size_t i = 3; i < points.size() - 1; ++i)
	{
		while (ccw(hull.second_to_top(), hull.top(), points[i]) <= 0.0)
		{
			hull.pop();
		}
		hull.push(points[i]);
	}
	cout << "Hull:" << endl;
	while (!hull.empty())
	{
		cout << hull.top() << endl;
		hull.pop();
	}
}

int main()
{
	Graham_demo();
	return 0;
}