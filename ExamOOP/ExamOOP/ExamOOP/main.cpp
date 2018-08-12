#include <iostream>
#include "WordsFactory.h"
#include "Document.h"
#include <random>
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
	auto filtered = doc.filter_messages("FmI");
	cout << "Filtered with FmI" << endl;
	cout << filtered << endl;
	return 0;
}