#include <iostream>
#include "WordsFactory.h"
#include "Document.h"
using namespace WordsFactory;
using std::cout;
using std::endl;

int main()
{
	Document doc(new Concrete_WordFactory());
	doc.load("test.txt");
	cout << "-- test.txt" << endl;
	std::cout << doc << std::endl;
	auto & msg = doc["fmi"]; // closest to fmi
	cout << "Compared to fmi" << endl;
	cout << msg << endl;
	
	return 0;
}