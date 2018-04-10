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
	void removeAt(int at);
	void addLine(Line * l);
	char * pop();
private:
	Node * root;
	Node * last;
	Node * getNodeAt(int at);
};
