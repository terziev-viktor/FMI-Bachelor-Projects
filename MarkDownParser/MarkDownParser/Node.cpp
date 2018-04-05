#include "Node.h"

Node::Node()
{
	this->next = nullptr;
	this->prev = nullptr;
}

Node::~Node()
{
	delete this->next;
	delete this->prev;
	delete this->line;
}

Node * Node::getNext()
{
	return this->next;
}

Node * Node::getPrev()
{
	return this->prev;
}

void Node::setNext(Node * nexLine)
{
	this->next = nexLine;
}

void Node::setPrev(Node * prev)
{
	this->prev = prev;
}

Line * Node::getLine()
{
	return this->line;
}

void Node::setLine(Line & l)
{
	this->line = new Line(l);
}
