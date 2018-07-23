#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

// Dummy class with some properties
class A
{
public:
	A(int prop1, double prop2, char prop3)
		:prop1(prop1),prop2(prop2),prop3(prop3)
	{	}

	int prop1;
	double prop2;
	char prop3;
};

static const unsigned int SIZE = 4; // size of dynamically allocated array (using malloc)
static A * memory = (A*)malloc(SIZE * sizeof(A)); // memory block to use in the program

ostream & operator<<(ostream & os, const A & a)
{
	os << a.prop1 << ' ' << a.prop2 << ' ' << a.prop3;
	return os;
}

int main()
{
	if (!memory) // malloc returns null if the allocation fails
	{
		return 1;
	}
	for (size_t i = 0; i < SIZE; i++)
	{
		new(memory + i) A(i,3.14159 + (double)i, 'A' + i); // calling c-tor of the class on the already allocated memory 
	}

	for (size_t i = 0; i < 4; i++)
	{
		cout << memory[i] << endl; // printing the result too see if it works or not
	}
	free(memory); // deallocating memory block
	return 0;
}
