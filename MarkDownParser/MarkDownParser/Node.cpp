#include "Node.h"

Node::Node()
{
	this->next = nullptr;
	this->line = nullptr;
}

Node::~Node()
{
	if (this->line != nullptr)
	{
		delete this->line;
	}
}

Node * Node::getNext()
{
	return this->next;
}

void Node::setNext(Node * nexLine)
{
	this->next = nexLine;
}

Line * Node::getLine()
{
	return this->line;
}

void Node::setLine(Line * l)
{
	this->line = l;
}
