#include <iostream>
#include "Vector.hpp"

using std::cout;
using std::endl;

int main()
{
	Vector<double> a;
	a.push_back(4);
	a.push_back(4);
	a.push_back(4);
	a.push_back(4);
	a.push_back(4);

	Vector<double> b;
	b.push_back(2);
	b.push_back(2);
	b.push_back(2);
	b.push_back(2);
	b.push_back(2);
	b.push_back(2);
	b.push_back(2);

	return 0;
}