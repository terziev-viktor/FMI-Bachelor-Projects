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
	Vector<Hashtag> hashtags = doc.get_hashtags();
	cout << "-- Hashtags" << endl;
	for (size_t i = 0; i < hashtags.count(); ++i)
	{
		cout << hashtags[i].get_value() << " " << endl;
	}
	return 0;
}