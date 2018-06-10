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

	Vector<double>::VectorIterator i = a.createIterator();
	while (!i.isDone())
	{
		cout << *i << endl;
		bool r = i < 3;
		cout << "i < 10? " << r << endl;
		++i;
	}

	cout << b << endl;

	b /= a;
	cout << b << endl;
	a *= b;
	cout << a << endl;
	a *= 3.14;
	cout << a << endl;
	a /= 2;
	cout << a << endl;
	a -= 1;
	cout << a << endl;
	a += 20;
	cout << a << endl;
	a++;
	cout << "Integral of a = " << a << endl;
	a--;
	cout << "Derivative of the integral of a = a = " << a << endl;
	cout << "a(12)=";
	double r = a(12);
	cout << r << endl;

	return 0;
}