#include <iostream>
#include "Vector.hpp"

using std::cout;
using std::endl;

int main()
{
	// The class is called vector and not polynom because the polynom is represented by a vector
	// that has some methods that allow it to act as a polynom. Such methods are:
	// Vector<T>::operator(T x) - value of the polynom in point x
	// Vector<T>::operator++() - integral of the polynom
	// Vector<T>::operator--() - derivative of the polynom
	// ...
	// and others (required by the assignment)
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

	Vector<int> c(3, 1, 2, 3);
	cout << "Many arguments (3):" << endl;
	cout << c << endl;
	cout << endl;
	Vector<double>::VectorIterator i = a.createIterator();
	while (!i.isDone())
	{
		cout << *i << endl;
		bool r = i < 3;
		cout << "i < 3? " << (r?"true":"false") << endl;
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
	cout << "Derivative of the integral of a = " << a << endl;
	cout << "a(12)=";
	double r = a(12);
	cout << r << endl;

	return 0;
}