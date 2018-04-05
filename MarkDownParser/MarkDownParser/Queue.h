#pragma once
#include "Node.h"

class Queue
{
public:
	Queue();
	~Queue();
	Line * getLine(int at);
	Line * getFirst();
	Line * getLast();
	void addLine(Line & l);
private:
	Node * root;
	Node * last;
};
