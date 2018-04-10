#include "Queue.h"



Queue::Queue()
{
	this->last = nullptr;
	this->root = nullptr;
}


Queue::~Queue()
{
	if (this->root != nullptr)
	{
		delete this->root;
	}
}

Line * Queue::getLine(int at)
{
	Node * n = this->getNodeAt(at);
	if (n != nullptr)
	{
		return n->getLine();
	}
	return nullptr;
}

Line * Queue::getFirst()
{
	return this->root->getLine();
}

Line * Queue::getLast()
{
	return this->last->getLine();
}

void Queue::removeAt(int at)
{
	if (at == 0)
	{
		Node * n = this->root->getNext();
		this->root->setNext(nullptr);
		delete this->root;
		this->root = n;
	}
	Node * n = this->getNodeAt(at - 1);
	Node * n1 = n->getNext();
	Node * n2 = n1->getNext();

	n->setNext(n2);
	n1->setNext(nullptr);
	delete n1;
}

void Queue::addLine(Line * l)
{
	Node * n = new Node();
	n->setLine(l);
	n->setNext(nullptr);
	if (this->root == nullptr)
	{
		this->root = n;
		this->last = n;
		return;
	}
	this->last->setNext(n);
	this->last = n;
}

char * Queue::pop()
{
	if (this->root == nullptr)
	{
		return nullptr;
	}
	Line * line = this->root->getLine();
	int size = line->getSize();
	char * content = new char[line->getSize()];
	for (int i = 0; i < size; i++)
	{
		content[i] = line->getChar(i);
	}
	Node * newroot = this->root->getNext();
	delete this->root;
	this->root = newroot;
	return content;
}

Node * Queue::getNodeAt(int at)
{
	if (this->root == nullptr)
	{
		return nullptr;
	}
	if (at < 0)
	{
		return nullptr;
	}
	Node * n = this->root;
	for (int i = 0; i < at; i++)
	{
		if (n->getNext() == nullptr)
		{
			break;
		}
		n = n->getNext();
	}
	return n;
}
