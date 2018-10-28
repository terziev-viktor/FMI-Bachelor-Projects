#pragma once
template<class T>
class singly_linked_list
{
private:
	class node
	{
	public:
		T data;
		node * next;

		node(const T & data, node * next)
		{
			this->data = data;
			this->next = next;
		}
		~node()
		{
			if (next)
			{
				delete next;
			}
		}
	};

	node * first;
	node * last;
	size_t count;
public:
	singly_linked_list()
		:first(nullptr),last(nullptr),count(0)
	{

	}

	~singly_linked_list()
	{
		if (this->first)
		{
			delete this->first;
		}
	}

	size_t get_count() const
	{
		return this->count;
	}

	const T & operator[](size_t at) const
	{
		if (at >= count)
		{
			throw "out of range";
		}
		if (at == 0)
		{
			return this->first->data;
		}
		if (at == this->count - 1)
		{
			return this->last->data;
		}
		node * i = this->first;
		while (at > 0)
		{
			i = i->next;
			--at;
		}
		return i->data;
	}

	T & operator[](size_t at)
	{
		if (at >= count)
		{
			throw "out of range";
		}
		if (at == 0)
		{
			return this->first->data;
		}
		if (at == this->count - 1)
		{
			return this->last->data;
		}
		node * i = this->first;
		while (at > 0)
		{
			i = i->next;
			--at;
		}
		return i->data;
	}

	singly_linked_list<T> & push_back(const T & value)
	{
		if (last)
		{
			node * next = new node(value, nullptr);
			this->last->next = next;
			this->last = next;
		}
		else
		{
			this->first = new node(value, nullptr);
			this->last = this->first;
		}
		++this->count;
		return *this;
	}

	singly_linked_list<T> & push_front(const T & value)
	{
		if (first)
		{
			node * prev = new node(value, this->first);
			this->first = prev;
		}
		else
		{
			this->first = new node(value, nullptr);
			this->last = this->first;
		}
		++this->count;
		return *this;
	}

	singly_linked_list<T> & remove_front()
	{
		node * tmp = this->first;
		this->first = this->first->next;
		tmp->next = nullptr;
		delete tmp;
		--this->count;
		return *this;
	}
};


