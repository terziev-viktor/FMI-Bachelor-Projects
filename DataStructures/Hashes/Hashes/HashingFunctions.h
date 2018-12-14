#pragma once
#include <string>

// http://www.cse.yorku.ca/~oz/hash.html
size_t GoodHashingFunction(const std::string & a)
{//                       * *  **
	size_t hash = 5381; // *magic* number
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

size_t RandomHashingFunction(const std::string & a)
{
	return rand();
}
