#pragma once
#include <iostream>

template<typename T>
class DoublyLinkedList
{
protected:
	// Each node is responsible for the deletion of the next (if such exists)
	class Node
	{
	private:
		Node * next;
		Node * prev;
		T data;
	public:
		Node()
			:next(nullptr)
		{

		}

		Node(const T & value)
			:next(nullptr),prev(nullptr)
		{
			this->setValue(value);
		}

		~Node()
		{
			if (this->next)
			{
				delete this->next;
			}
		}

		void setValue(const T & value) { this->data = value; }

		T getValue() const { return this->data;	}

		T & getValue() { return this->data;	}

		void setNext(Node * next)
		{
			if (this == next)
			{
				throw "no logic exception";
			}
			this->next = next;
		}

		void setPrev(Node * prev)
		{
			if (this == prev)
			{
				throw "no logic exception";
			}
			this->prev = prev;
		}

		Node * getPrev() { return this->prev; }

		const Node * getPrev() const { return this->prev; }

		const Node * getNext() const { return this->next; }

		Node * getNext() { return this->next; }

		bool hasNext() const { return this->next != nullptr; }

		bool hasPrev() const { return this->prev != nullptr; }
	};

private:
	Node * first;
	Node * last;
	size_t count;
public:
	class Iterator
	{
	private:
		Node * start;
		Node * current;
		bool done;
	public:
		Iterator(Node * start)
			:start(start), done(false)
		{
			this->current = start;
		}

		bool isDone() const	{ return this->done; }

		const T & getCurrent() const { return this->current->getValue(); }

		const T & getFirst() const { this->start->getValue(); }

		operator bool() const	{ return !this->done; }

		const T & operator*() const	{ return this->current->getValue(); }

		Iterator & operator--()	
		{ 
			this->current = this->current->getPrev();
			this->done = !this->current->hasPrev();
			return *this;
		}

		Iterator & operator--(int)
		{
			Iterator tmp(this->current);
			this->current = this->current->getPrev();
			this->done = !this->current->hasPrev();
			return tmp;
		}

		Iterator & operator++()
		{
			this->current = this->current->getNext();
			this->done = !this->current->hasNext();
			return *this;
		}

		Iterator operator++(int)
		{
			Iterator tmp(this->current);
			this->current = this->current->getNext();
			this->done = !this->current->hasNext();
			return tmp;
		}
	};

	DoublyLinkedList()
		:first(nullptr), last(nullptr), count(0)
	{ }

	~DoublyLinkedList()
	{
		if (this->first)
		{
			delete this->first;
		}
	}

	T getFirst() const
	{
		return this->first->getValue();
	}

	T getLast() const
	{
		return this->last->getValue();
	}

	T getAt(size_t index) const
	{
		Node * tmp = this->first;
		for (size_t i = 0; i < index; ++i)
		{
			if (tmp->getNext() == nullptr)
			{
				throw "Invalid Index";
			}
			tmp = tmp->getNext();
		}
		return tmp->getValue();
	}

	size_t getCount() const
	{
		return this->count;
	}

	DoublyLinkedList & pushBack(const T & value)
	{
		if (this->first == nullptr)
		{
			this->first = new Node(value);
			this->last = this->first;
		}
		else
		{
			Node * newNode = new Node(value);
			this->last->setNext(newNode);
			newNode->setPrev(this->last);
			this->last = newNode;

		}
		++this->count;
		return *this;
	}

	DoublyLinkedList & pushFront(const T & value)
	{
		if (this->first == nullptr)
		{
			this->first = new Node(value);
			this->last = this->first;
		}
		else
		{
			Node * newNode = new Node(value);
			newNode->setNext(this->first);
			this->first->setPrev(newNode);
			this->first = newNode;
		}
		++this->count;
		return *this;
	}

	DoublyLinkedList & removeFront()
	{
		if (this->first == nullptr)
		{
			return *this;
		}
		Node * tmp = this->first;
		if (this->first->getNext() != nullptr)
		{
			this->first = this->first->getNext();
		}
		tmp->setNext(nullptr);
		delete tmp;
		--this->count;
		return *this;
	}

	DoublyLinkedList & removeBack()
	{
		if (this->last == nullptr)
		{
			return *this;
		}
		Node * tmp = this->last;
		if (this->last->getPrev() != nullptr)
		{
			this->last = this->last->getPrev();
		}
		tmp->setNext(nullptr);
		delete tmp;
		--this->count;
		return *this;
	}

	// Returns Iterator<T> starting at the beginning of the DoublyLinkedList<T>
	Iterator begin() const
	{
		return Iterator(this->first);
	}

	// Returns Iterator<T> starting at the end of the DoublyLinkedList<T>
	Iterator end() const
	{
		return Iterator(this->last);
	}

};