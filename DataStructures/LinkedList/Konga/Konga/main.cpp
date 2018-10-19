#include <iostream>
#include "SinglyLinkedList.hpp"

using namespace std;

int main()
{
	SinglyLinkedList<double> list;
	list.Add(222.2).Add(33.3).Add(1.1);
	double a = list.getAt(2);
	cout << a << endl;

	return 0;
}