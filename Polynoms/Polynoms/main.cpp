#include <iostream>
#include "Vector.hpp"

using std::cout;
using std::endl;

int main()
{
	Vector<int> a;
	a.push_back(2);
	a.push_back(3);

	Vector<int> b;
	b.push_back(2);
	b.push_back(3);
	b.push_back(4);
	b.push_back(4);
	b.push_back(100);
	b.push_back(4);
	b.push_back(4);
	b.push_back(4);
	b.push_back(4);
	b.push_back(4);
	b.push_back(4);
	b.push_back(4);
	b.push_back(4);

	int x = a(2);
	cout << x << endl;
	bool asBool = (bool)a;
	int asInt = (int)a;
	cout << "as bool: " << asBool << endl;
	cout << "as int: " << asInt << endl;
	return 0;
}