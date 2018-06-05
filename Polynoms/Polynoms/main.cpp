#include <iostream>
#include "Vector.hpp"

using std::cout;
using std::endl;

int main()
{
	Vector<double> a;
	a.push_back(2);
	a.push_back(3);
	a.push_back(3);
	a.push_back(3);
	a.push_back(3);
	a.push_back(3);
	a.push_back(3);
	a++;
	IIterator<double> * iterator = a.createIterator();

	while (!iterator->isDone())
	{
		double current = iterator->current();
		cout << current << " ";
		iterator->next();
	}
	delete iterator;

	return 0;
}