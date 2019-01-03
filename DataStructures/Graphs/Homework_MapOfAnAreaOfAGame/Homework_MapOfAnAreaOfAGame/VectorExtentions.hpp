#pragma once
#include <vector>
using std::vector;

template<typename T>
bool vector_contains(const vector<T> & vect, bool(*condition)(const T &))
{
	for (const T & item : vect)
	{
		if (condition(item))
		{
			return true;
		}
	}
	return false;
}
