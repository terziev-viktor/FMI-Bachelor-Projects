#pragma once

template<typename T>
class LinkedList
{
public:
	struct Node
	{
		T data;
		Node * next;
		Node():next(nullptr){}
		~Node()
		{
			if (next)
			{
				delete next;
			}
		}
	};

	LinkedList()
		:first(nullptr), last(nullptr), size(0)
	{
		
	}

	~LinkedList()
	{
		if (first)
		{
			delete first;
		}
	}

	LinkedList(const LinkedList<T> & other);

	LinkedList<T> & operator=(const LinkedList<T> & other);

	class Iterator
	{
	private:
		Node * begin;
		Node * last; // acts like current element while iterating
	public:
		Iterator()
			:begin(nullptr), last(nullptr)
		{

		}

		Iterator(Node * begin)
		{
			this->begin = begin;
			this->last = begin;
		}

		bool Done() const
		{
			return this->last == nullptr;
		}

		bool operator==(const Iterator & other)
		{
			return this->last == other.last;
		}

		Node * Current() const { return last; }

		Iterator & operator++()
		{
			this->last = this->last->next;
			return *this;
		}

		Iterator operator++(int)
		{
			Iterator old(*this);
			this->last = this->last->next;
			return old;
		}

		const T & operator*() const
		{
			return this->last->data;
		}

		T & operator*()
		{
			return this->last->data;
		}

		Node * operator->()
		{
			return this->last;
		}

		void Rewind()
		{
			this->last = this->begin;
		}
	};
	
	void SwapWith(LinkedList<T> & other)
	{
		Node * tmp = this->first;
		this->first = other.first;
		other.first = tmp;
		tmp = this->last;
		this->last = other.last;
		other.last = tmp;
		size_t tmpSize = this->size;
		this->size = other.size;
		other.size = tmpSize;
	}

	bool Empty() const
	{
		return this->size == 0;
	}

	Iterator Begin() const
	{
		return Iterator(this->first);
	}

	size_t Size() const { return size; }

	void AddBack(const T & data)
	{
		if (this->first == nullptr)
		{
			this->first = new Node();
			this->first->data = data;
			this->last = this->first;
		}
		else
		{
			Node * p = new Node();
			p->data = data;
			this->last->next = p;
			this->last = p;
		}
		++size;
	}

	void AddFront(const T & data)
	{
		if (this->first == nullptr)
		{
			this->first = new Node();
			this->first->data = data;
			this->last = this->first;
		}
		else
		{
			Node * p = new Node();
			p->data = data;
			p->next = this->first;
			this->first = p;
		}
		++size;
	}

	const T & GetLast() const
	{
		return this->last->data;
	}

	T & GetLast()
	{
		return this->last->data;
	}

	const T & GetFirst() const
	{
		return this->first->data;
	}

	T & GetFirst()
	{
		return this->first->data;
	}

	void RemoveFirst()
	{
		if (this->first)
		{
			Node * p = this->first;
			this->first = this->first->next;
			p->next = nullptr;
			delete p;
			--size;
		}
		if (this->first == nullptr)
		{
			this->last = nullptr;
		}
	}

	void Remove(const Iterator & i)
	{
		Node * p = this->first;
		Node * p1 = i.Current();
		if (p == p1)
		{
			this->RemoveFirst();
			return;
		}
		while (p && p->next && p->next != p1)
		{
			p = p->next;
		}
		p->next = p1->next;
		p1->next = nullptr;
		delete p1;
		--size;
	}

	void Print(std::ostream & out) const
	{
		for (LinkedList<T>::Iterator i = this->Begin(); !i.Done(); ++i)
		{
			out << *i << '\n';
		}
	}
protected:
	Node * first;
	Node * last;
	size_t size;
};

template<typename T>
inline LinkedList<T>::LinkedList(const LinkedList<T> & other)
{
	this->size = 0;
	for (Iterator it = other.Begin(); !it.Done(); ++it)
	{
		this->AddBack(*it);
	}
}

template<typename T>
inline LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& other)
{
	if (this->first)
	{
		delete this->first;
		this->first = nullptr;
		this->last = nullptr;
	}
	for (Iterator it = other.Begin(); !it.Done(); ++it)
	{
		this->AddBack(*it);
	}
	return *this;
}
