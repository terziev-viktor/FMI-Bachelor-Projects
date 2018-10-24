#pragma once

template<typename T>
class SinglyLinkedList
{
protected:
	class Node
	{
	private:
		Node * next;
		T data;
	public:
		Node()
			:next(nullptr)
		{

		}
		Node(const T & value)
		{
			this->setValue(value);
			this->next = nullptr;
		}
		~Node()
		{
			if (this->next)
			{
				delete this->next;
			}
		}

		void setValue(const T & value)
		{
			this->data = value;
		}

		T getValue() const
		{
			return this->data;
		}

		T & getValue()
		{
			return this->data;
		}

		void setNext(Node * next)
		{
			if (this == next)
			{
				throw "no logic exception";
			}
			this->next = next;
		}
		Node * getNext() const
		{
			return this->next;
		}
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
		{
			this->start = start;
			this->current = this->start;
			this->done = false;
		}
		bool isDone() const
		{
			return this->done;
		}
		T & getCurrent()
		{
			return this->current->getValue();
		}
		T getFirst() const
		{
			this->start->getValue();
		}
		bool operator()() const
		{
			return !this->done;
		}
		T operator*() const
		{
			return this->current->getValue();
		}
		T & operator*()
		{
			return this->current->getValue();
		}
		Iterator & operator->()
		{
			return *this;
		}
		
		Iterator & operator++()
		{
			if (this->current == nullptr || this->current->getNext() == nullptr)
			{
				this->done = true;
				return *this;
			}
			this->current = this->current->getNext();
			return *this;
		}
		Iterator operator++(int)
		{
			if (this->current == nullptr || this->current->getNext() == nullptr)
			{
				this->done = true;
				return *this;
			}
			Iterator tmp(this->current);
			this->current = this->current->getNext();
			return tmp;
		}

	};

	SinglyLinkedList()
		:first(nullptr), last(nullptr), count(0)
	{

	}

	~SinglyLinkedList()
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

	SinglyLinkedList & pushBack(const T & value)
	{
		if (first == nullptr)
		{
			this->first = new Node(value);
			this->last = this->first;
		}
		else
		{
			this->last->setNext(new Node(value));
			this->last = this->last->getNext();
		}
		++this->count;
		return *this;
	}

	SinglyLinkedList & pushFront(const T & value)
	{
		if (this->first == nullptr)
		{
			this->first = new Node(value);
			this->last = this->first;
		}
		else
		{
			Node * tmp = new Node(value);
			tmp->setNext(this->first);
			this->first = tmp;
		}
		+this->count;
		return *this;
	}

	SinglyLinkedList & removeFront()
	{
		Node * tmp = this->first;
		this->first = this->first->getNext();
		tmp->setNext(nullptr);
		delete tmp;
		return *this;
	}

	Iterator begin() const
	{
		return Iterator(this->first);
	}

	// Implemented just for fun ----------------
	void forEach(void(*func)(T & n))
	{
		for (Iterator i = this->begin(); !i.isDone(); ++i)
		{
			func(*i);
		}
	}

	bool findFirst(bool(*condition)(const T & n), T & out) const
	{
		for (Iterator i = this->begin(); !i.isDone(); ++i)
		{
			if (condition(*i))
			{
				out = *i;
				return true;
			}
		}
		return false;
	}
	// --------------------------------------------
};
