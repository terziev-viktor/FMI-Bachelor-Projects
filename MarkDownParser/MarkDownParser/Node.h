#pragma once
#include "Line.h"
class Node
{
public:
	Node();
	~Node();
	Node * getNext();
	void setNext(Node * next);
	Line * getLine();
	void setLine(Line * l);
private:
	Line * line;
	Node * next;
};

