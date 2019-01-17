#include <iostream>
#include "../Algorithms/vecta.h"
#include "../Algorithms/STDFunctions.h"
using namespace vecta;
using namespace std;

struct Line
{
	Point A;
	Point B;
};

struct Circle
{
	Point center;
	double radius;
};

ostream & operator<<(ostream & out, const Line & obj)
{
	cout << obj.A << "---" << obj.B;
	return out;
}

bool Intercept(const Line & l1, const Line & l2)
{
	return ccw(l1.A, l1.B, l2.A) * ccw(l1.A, l1.B, l2.B) < 0.0;
}

Circle Welzl(PointsSet & P, PointsSet & R)
{
	if (P.empty() || R.size() >= 3)
	{
		if (R.size() == 1)
		{
			return Circle() =
			{
				R[0], 0.0
			};
		}
		else if (R.size() == 2)
		{
			Point p1 = R[0];
			Point p2 = R[2];
			Point center((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
			double rad = (len(vec2d<double>(p1.x, p1.y) - vec2d<double>(p2.x, p2.y))) / 2;
			return Circle() =
			{
				center, rad
			};
		}
	}
}

int main()
{
	Point A1(1, 1), A2 (2, 2), B1(0, 1), B2(1, 0);
	Line line1 = 
	{
		A1, A2
	};
	Line line2 = 
	{
		B1, B2
	};
	Line line3 =
	{
		Point(0, 3), Point(4, 7)
	};

	cout << line1 << endl << line2 << endl;

	cout << "Intercept ? " << (Intercept(line1, line3) ? "yes" : "no") << endl;


	return 0;
}