#pragma once
#include <stack>
using std::stack;

template<typename T>
class cool_stack : public stack<T>
{
public:
	T second_to_top()
	{
		return this->c[this->size() - 2];
	}
};

