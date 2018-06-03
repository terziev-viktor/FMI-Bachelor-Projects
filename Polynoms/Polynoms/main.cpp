#include "Vector.hpp"
using namespace polynoms::tools;

#include <iostream>
using std::cout;
using std::endl;

int main()
{
	Vector<int> a;
	a.add(1);
	a.add(2);
	a.add(3);
	a.add(a.getAt(0) + a.getAt(1));
	a.print();
	
	return 0;
}