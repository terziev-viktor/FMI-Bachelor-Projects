#include "vecta.h"

#include <iostream>
#include <vector>

using namespace vecta;

int Sign(double a)
{
	if (!a)
		return 0;
	if (a > 0)
		return 1;
	else
		return -1;
}

double OrientedTriangleArea(vec2d<> A, vec2d<> B, vec2d<> C)
{
	return ((B - A) ^ (C - A)) / 2;
}

double OrientedPolygonArea(std::vector<vec2d<>> polygonPoints)
{
	double resultArea = 0;
	for (int i = 0; i < polygonPoints.size() - 1; i++)
	{
		resultArea += polygonPoints[i] ^ polygonPoints[i + 1];
	}
	return resultArea;
}

int TriangleOrientation(vec2d<> A, vec2d<> B, vec2d<> C)
{
	return Sign(OrientedTriangleArea(A, B, C));
}

int PolygonOrientation(std::vector<vec2d<>> polygonPoints)
{
	return Sign(TriangleOrientation(polygonPoints[0], polygonPoints[1], polygonPoints[2]));
}

vec2d<> FindLinesInterPt(vec2d<> line1Start, vec2d<> line1Vec, vec2d<> line2Start, vec2d<> line2Vec)
{
	double s	= line1Start^line1Vec;
	double t	= line2Vec^ line2Start;
	double uxv	= line1Vec^line2Vec;
	return vec2d<>( (s*line2Vec.x - t*line1Vec.x) / uxv, (s*line2Vec.y - t*line1Vec.y) / uxv);
}

vec2d<> FindSegmentsInterPt(vec2d<> p1, vec2d<> p2, vec2d<> q1, vec2d<> q2) // не е тестван
{
	if ((OrientedTriangleArea(p1, p2, q1) * OrientedTriangleArea(p1, p2, q2) < 0) && (OrientedTriangleArea(q1, q2, p1) * OrientedTriangleArea(q1, q2, p2) < 0))
	{
		double t = OrientedTriangleArea(q1, q2, p1) / (OrientedTriangleArea(q1, q2, p1) + OrientedTriangleArea(q1, q2, p2));
		return vec2d<>(p1.x + t * (p2 - p1).x, p1.y + t * (p2 - p1).y);
	}
	else
	{
		return vec2d<>(INFINITY, INFINITY);
	}
}

double OrientedDistance (vec2d<> linePoint, vec2d<> lineVec, vec2d<> point) //не е тестван
{
	return unit(lineVec) ^ (point - linePoint);
}

vec2d<> PointProjection(vec2d<> linePoint, vec2d<> lineVec, vec2d<> point)
{
	lineVec		= unit(lineVec);
	double t	= lineVec * (point - linePoint);
	return vec2d<>(linePoint.x + t*lineVec.x, linePoint.y + t*lineVec.y);
}

bool IsPointInsideTriangle(vec2d<> A, vec2d<> B, vec2d<> C, vec2d<> P)
{
	double a = OrientedTriangleArea(P, B, C) / OrientedTriangleArea(A, B, C);
	double b = OrientedTriangleArea(A, P, C) / OrientedTriangleArea(A, B, C);

	return a >= 0 && a <= 1 && b >= 0 && b <= 1;
}

bool IsPointInsideConvexPolygon(std::vector<vec2d<>> polygonPoints, vec2d<> P, int first, int last) //изпъкнал многоъгълник
{
	if (last - first == 2)	//last == first + 2
	{
		return IsPointInsideTriangle(polygonPoints[first], polygonPoints[first + 1], polygonPoints[last], P);
	}
	if (	TriangleOrientation(polygonPoints[first],		polygonPoints[first + 1], P)*PolygonOrientation(polygonPoints) < 0
		||	TriangleOrientation(polygonPoints[first + 1],	polygonPoints[first + 2], P)*PolygonOrientation(polygonPoints) < 0)
	{
		return false;
	}

	int mid = (last - first) / 2;

	if (TriangleOrientation(polygonPoints[mid], polygonPoints[first + 1], P)*PolygonOrientation(polygonPoints) > 0)
	{
		return IsPointInsideConvexPolygon(polygonPoints, P, mid, last);
	}	
	if (TriangleOrientation(polygonPoints[first + 1], polygonPoints[mid], P)*PolygonOrientation(polygonPoints) > 0)
	{
		return IsPointInsideConvexPolygon(polygonPoints, P, first, mid);
	}

}

bool IsPointInsideArbitraryPolygon(std::vector<vec2d<>> polygonPoints, vec2d<> P)
{
	int br = 0;

	for (int i = 0; i < polygonPoints.size() - 1; i++)
	{
		if (polygonPoints[i].y == P.y && polygonPoints[i + 1].y == P.y && (P.x - polygonPoints[i].x)*(P.x - polygonPoints[i + 1].x) < 0)
			return true;

		if (polygonPoints[i].y == P.y && polygonPoints[i + 1].y == P.y)
			continue;

		if (polygonPoints[i].y < P.y && P.y < polygonPoints[i + 1].y && OrientedTriangleArea(polygonPoints[i], polygonPoints[i + 1], P) > 0)
		{
			br++;
		}
		if (polygonPoints[i + 1].y < P.y && P.y < polygonPoints[i].y && OrientedTriangleArea(polygonPoints[i], polygonPoints[i + 1], P) < 0)
		{
			br++;
		}

		if (polygonPoints[i + 1].y == P.y)
			br++;
	}

	return br % 2 == 1;
}

int main()
{
	std::cout << OrientedTriangleArea(vec2d<>(0, 0), vec2d<>(1, 0), vec2d<>(0, 1)) << std::endl;
	std::cout << FindSegmentsInterPt(vec2d<>(0, 0), vec2d<>(1, 1), vec2d<>(1, 0), vec2d<>(-1, 1)).x << std::endl;

	std::vector<vec2d<>>	polygonPoints;
	std::cout << IsPointInsideConvexPolygon(polygonPoints, vec2d<>(0, 0), 0, polygonPoints.size() - 1) << std::endl;

	system("pause");
	return 0;
}