#pragma once
#include "Line.h"
class Node
{
public:
	Node();
	~Node();
	Node * getNext();
	Node * getPrev();
	void setNext(Node * next);
	void setPrev(Node * prev);
	Line * getLine();
	void setLine(Line & l);
private:
	Line * line;
	Node * next;
	Node * prev;
};

