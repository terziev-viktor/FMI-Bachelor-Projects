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
	std::cout << doc << std::endl;
	Vector<Hashtag> hashtags = doc.get_hashtags();

	return 0;
}