#pragma once
template<class T>
class doubly_linked_list
{
private:
	class node
	{
	public:
		T data;
		node * next;
		node * prev;
		node(const T & data, node * next)
		{
			this->data = data;
			this->next = next;
		}
		node(const T & data)
		{
			this->data = data;
			this->next = nullptr;
			this->prev = nullptr;
		}
		~node()
		{
			if (this->next)
			{
				delete this->next;
			}
		}
	};

	node * first;
	node * last;
	size_t count;
public:
	doubly_linked_list()
		:first(nullptr),last(nullptr),count(0)
	{

	}

	~doubly_linked_list()
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

	doubly_linked_list<T> & push_back(const T & value)
	{
		if (last)
		{
			node * next = new node(value, nullptr);
			next->prev = this->last;
			this->last->next = next;
			this->last = next;
		}
		else
		{
			this->first = new node(value);
			this->last = this->first;
		}
		++this->count;
		return *this;
	}

	doubly_linked_list<T> & push_front(const T & value)
	{
		if (first)
		{
			node * prev = new node(value, this->first);
			this->first->prev = prev;
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

	doubly_linked_list<T> & remove_front()
	{
		node * tmp = this->first;
		this->first = this->first->next;
		this->first->prev = nullptr;
		tmp->next = nullptr;
		delete tmp;
		--this->count;
		return *this;
	}

	doubly_linked_list<T> & remove_back()
	{
		node * tmp = this->last;
		this->last = this->last->prev;
		delete tmp;
		this->last->next = nullptr;
		--this->count;
		return *this;
	}
};


