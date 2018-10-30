#pragma once
#include <stack>
using std::stack;

template<typename T>
class cool_stack : public stack<T>
{
public:
	T second_to_top()
	{
		T a = this->top();
		this->pop();
		T b = this->top();
		this->pop();
		this->push(b);
		this->push(a);
		return b;
	}
};

