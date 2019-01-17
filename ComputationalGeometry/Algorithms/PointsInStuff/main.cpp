#include "../Algorithms/STDFunctions.h"
bool PointInPoly(const PointsSet & polygon, const Point & P)
{
	size_t c = 0;
	for (size_t i = 0; i < polygon.size() - 1; ++i)
	{
		const Point & A = polygon[i];
		const Point & B = polygon[i + 1];

		bool is_ccw = ccw(A, B, P);

		if (P.y == A.y && P.y == B.y)
		{
			if ((P.x >= A.x && P.x <= B.x) || (P.x <= A.x && P.x >= B.x))
			{
				return true;
			}
			continue;
		}

		if (A.y > B.y && (P.y < A.y && P.y > B.y) && is_ccw)
		{	
			++c; continue;
		}

		if (B.y > A.y && (P.y > A.y && P.y < B.y) && !is_ccw)
		{
			++c; continue;
		}

		if ((P.y == A.y && A.y < B.y && is_ccw) ||
			(P.y == B.y && B.y < A.y && !is_ccw))
		{
			++c;
		}
	}
	return c % 2 == 1;
}



double PolyOrient(const PointsSet & poly)
{
	return ccw(poly[0], poly[1], poly[2]) / 2.0;
}

double TriangleOriant(const Point & A, const Point & B, const Point & C)
{
	return ccw(A, B, C) / 2.0;
}

bool PointInTriangle(const Point & A, const Point & B, const Point & C, const Point & P)
{
	double abc = TriangleOriant(A, B, C);
	double a = TriangleOriant(A, B, P) / abc;
	double b = TriangleOriant(A, P, C) / abc;
	return a >= 0.0 && a <= 1.0 && b >= 0.0 && b <= 1.0 && a + b <= 1.0;
}

bool PointInConvexPolySearch(const PointsSet & poly, const Point & p, size_t first, size_t last)
{
	if (last - first == 2)
	{
		return PointInTriangle(poly[first], poly[first + 1], poly[last], p);
	}
	if (PolyOrient(poly) * TriangleOriant(poly[first], poly[first + 1], p) < 0 ||
		PolyOrient(poly) * TriangleOriant(poly[first + 1], poly[first + 2] , p) < 0)
	{
		return false;
	}
	size_t mid = (last - first) / 2;
	if (TriangleOriant(poly[mid], poly[first + 1], p) * PolyOrient(poly) > 0.0)
	{
		return PointInConvexPolySearch(poly, p, mid, last);
	}
	if (TriangleOriant(poly[first + 1], poly[mid], p) * PolyOrient(poly) > 0.0)
	{
		return PointInConvexPolySearch(poly, p, first, mid);
	}
}

bool PointInConvexPoly(const PointsSet & poly, const Point & p)
{
	return PointInConvexPolySearch(poly, p, 0, poly.size() - 1);
}


int main()
{
	PointsSet triangle;
	Point p;
	cout << "A:";
	cin >> p.x;
	cin >> p.y;
	triangle.push_back(p);
	cout << "B:";
	cin >> p.x;
	cin >> p.y;
	triangle.push_back(p);
	cout << "C:";
	cin >> p.x;
	cin >> p.y;
	triangle.push_back(p);
	
	while (true)
	{
		cout << "P:";
		cin >> p.x;
		cin >> p.y;
		cout << (PointInTriangle(triangle[0], triangle[1], triangle[2], p) ? "true" : "false") << endl;
	}

	return 0;
}