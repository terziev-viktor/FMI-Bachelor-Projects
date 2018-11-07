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
			this->prev = nullptr;
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

	node * create_node(const T & data, node * next)
	{
		node * n = new(std::nothrow) node(data, next);
		while (!n)
		{
			n = new(std::nothrow) node(data, next);
		}
		return n;
	}

	node * get_node_at(size_t at)
	{
		if (at == 0)
		{
			return this->first;
		}
		node * i = this->first;
		while (at > 0)
		{
			if (i == nullptr)
			{
				throw std::out_of_range("Index out of range for get_node_at(size_t at)");
			}
			i = i->next;
			--at;
		}
		return i;
	}

	node * find_node(bool(*comparison)(const T & n))
	{
		node * i = this->first;
		while (!comparison(i->data))
		{
			i = i->next;
			if (i == nullptr)
			{
				return nullptr;
			}
		}
		return i;
	}

	void copy_from(const doubly_linked_list<T> & other)
	{
		for (iterator it = other.begin(); !it.is_done(); ++it)
		{
			this->push_back(*it);
		}
	}

public:

	class iterator
	{
		node * current;
	public:
		iterator(node * first)
		{
			this->current = first;
		}
		bool is_done() const
		{
			return this->current == nullptr;
		}

		const T & operator*() const { return this->current->data; }

		// ++it
		iterator & operator++()
		{
			this->current = this->current->next;
			return *this;
		}
		// it++
		iterator operator++(int)
		{
			iterator temp(this->current);
			this->current = this->current->next;
			return temp;
		}
	};

	class rev_iterator
	{
	private:
		node * current;
	public:
		rev_iterator(node * last)
		{
			this->current = last;
		}
		bool is_done() const
		{
			return this->current == nullptr;
		}

		const T & operator*() const { return this->current->data; }

		// --it
		rev_iterator & operator--()
		{
			this->current = this->current->prev;
			return *this;
		}
		// it++
		rev_iterator operator--(int)
		{
			rev_iterator temp(this->current);
			this->current = this->current->prev;
			return temp;
		}
	};

	doubly_linked_list()
		:first(nullptr),last(nullptr)
	{

	}

	doubly_linked_list(const doubly_linked_list & other)
	{
		this->copy_from(other);
	}

	doubly_linked_list<T> & operator=(const doubly_linked_list<T> & other)
	{
		if (this->first)
		{
			delete this->first;
		}
		this->first = nullptr;
		this->last = nullptr;
		this->copy_from(other);
	}

	~doubly_linked_list()
	{
		if (this->first)
		{
			delete this->first;
		}
	}

	bool empty() const
	{
		return this->first == nullptr;
	}

	iterator begin() const
	{
		return iterator(this->first);
	}

	rev_iterator begin_back() const
	{
		return rev_iterator(this->last);
	}
	
	const T & get_last() const { return this->last->data; }

	T get_last() { return this->last->data;	}

	const T & get_first() const { return this->first->data; }

	T get_first() { return this->first->data; }

	const T & operator[](size_t at) const
	{
		return get_node_at(at)->data;
	}

	T & operator[](size_t at)
	{
		return this->get_node_at(at)->data;
	}

	doubly_linked_list<T> & append(doubly_linked_list<T> & other)
	{
		this->last->next = other.first;
		other.first->prev = this->last;
		this->last = other.last;
		other.first = nullptr;
		other.last = nullptr;
		return *this;
	}

	// cuts this list from position <at> and returns a pointer to a new list storing the remaining elements
	doubly_linked_list<T> * cut(size_t at)
	{
		doubly_linked_list<T> * out = new (std::nothrow) doubly_linked_list<T>();

		while (!out)
		{
			out = new (std::nothrow) doubly_linked_list<T>();
		}

		if (at == 0)
		{
			out->first = this->first;
			out->last = this->last;
			this->first = nullptr;
			this->last = nullptr;
			return out;
		}
		node * this_new_last = this->get_node_at(at - 1);
		node * other_new_first = this_new_last->next;
		
		this_new_last->next = nullptr;
		other_new_first->prev = nullptr;

		out->first = other_new_first;
		out->last = this->last;

		this->last = this_new_last;
		return out;
	}

	doubly_linked_list<T> * cut(bool(*comparison)(const T &))
	{
		node * n = this->find_node(comparison);
		if (n == nullptr)
		{
			throw std::exception("Could not find element using comparison function");
		}
		doubly_linked_list<T> * out = new(std::nothrow) doubly_linked_list<T>();
		while (!out)
		{
			out = new(std::nothrow) doubly_linked_list<T>();
		}

		if (n->prev == nullptr)
		{
			out->first = this->first;
			out->last = this->last;
			this->last = nullptr;
			this->first = nullptr;
			return out;
		}
		if (n->next == nullptr)
		{
			return out;
		}
		out->first = n;
		out->last = this->last;
		this->last = n->prev;

		this->last->next = nullptr;
		out->first->prev = nullptr;

		return out;
	}

	doubly_linked_list<T> & push_back(const T & value)
	{
		if (last)
		{
			node * next = this->create_node(value, nullptr);
			next->prev = this->last;
			this->last->next = next;
			this->last = next;
		}
		else
		{
			this->first = this->create_node(value, nullptr);
			this->last = this->first;
		}
		return *this;
	}

	doubly_linked_list<T> & push_front(const T & value)
	{
		if (first)
		{
			node * prev = this->create_node(value, this->first);
			this->first->prev = prev;
			this->first = prev;
		}
		else
		{
			this->first = this->create_node(value, nullptr);
			this->last = this->first;
		}
		return *this;
	}

	doubly_linked_list<T> & remove_front()
	{
		node * tmp = this->first;
		this->first = this->first->next;
		if(this->first)
			this->first->prev = nullptr;
		tmp->next = nullptr;
		delete tmp;
		return *this;
	}

	doubly_linked_list<T> & remove_back()
	{
		node * tmp = this->last;
		this->last = this->last->prev;
		delete tmp;
		this->last->next = nullptr;
		return *this;
	}
};
