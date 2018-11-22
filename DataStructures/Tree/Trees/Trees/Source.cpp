#include "Tree.hpp"
#include <iostream>
using std::cin;
using std::cout;
using std::endl;

struct BigStruct
{
	int i;
	int data[1024];
};

struct IntComparator
{
	int operator()(const int & a, const int & b) const
	{
		return a - b;
	}
};

ostream & operator<<(ostream & os, const BigStruct & obj)
{
	os << obj.i;
	return os;
}

struct BigStructComparator
{
	int operator()(const BigStruct & a, const BigStruct & b)
	{
		return a.i - b.i;
	}
};

int main()
{
	Tree<BigStruct, BigStructComparator> tree;
	
	tree.print(cout);
	return 0;
}