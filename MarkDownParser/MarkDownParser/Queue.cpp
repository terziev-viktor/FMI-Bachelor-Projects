#include "Queue.h"



Queue::Queue()
{
}


Queue::~Queue()
{
}

Line * Queue::getLine(int at)
{
	Node * n = root;
	for (int i = 0; i < at; i++)
	{
		n = n->getNext();
		if (n == nullptr)
		{
			break;
		}
	}
	return n->getLine();

}

Line * Queue::getFirst()
{
	return this->root->getLine();
}

Line * Queue::getLast()
{
	return this->last->getLine();
}

void Queue::addLine(Line & l)
{
	Node * n = new Node();
	n->setLine(l);
	n->setNext(nullptr);
	n->setPrev(this->last);
	this->last = n;
}
