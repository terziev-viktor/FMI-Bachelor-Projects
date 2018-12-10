#include "HashTable.hpp"
#include <iostream>

// http://www.cse.yorku.ca/~oz/hash.html
size_t GoodHashingFunction(const std::string & a)
{
	size_t hash = 5381;
	int c;
	size_t i = 0;
	while (c = a[i++])
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}

size_t BadHashingFunction(const std::string & a)
{
	return 30;
}

int main()
{
	HashTable<std::string, int, BadHashingFunction> hsh;

	hsh.Insert("some", 12);
	hsh.Insert("s1me2", 0);
	hsh.Insert("sdfasf", 11);
	hsh.Insert("asgasf", 1111);

	hsh.Delete("some");

	hsh.Print(std::cout);

	return 0;
}