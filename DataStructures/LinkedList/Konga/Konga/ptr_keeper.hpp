#pragma once
#include <stdexcept>

//	Not exception safe class for keeping pointers to objects. The user is resposible for objects' lifes
template<class T>
class ptr_keeper
{
private:
	size_t size;
	size_t count;
	T * * buffer;
	size_t resize_rate = 2;

	void expand()
	{
		this->size *= this->resize_rate;
		T * * new_buffer = new T*[this->size];
		for (size_t i = 0; i < this->count; ++i)
		{
			new_buffer[i] = this->buffer[i];
		}
		delete[] this->buffer;
		this->buffer = new_buffer;
	}
public:
	ptr_keeper(size_t preserve_size = 15)
	{
		this->size = preserve_size;
		this->count = 0;
		this->buffer = new T*[this->size];
	}

	ptr_keeper(const ptr_keeper<T> & other)
	{
		this->size = other.get_size();
		this->count = other.get_count();
		this->resize_rate = other.get_resize_rate();
		this->buffer = new T*[this->size];
		// copying only the pointers not the elements because 
		// ptr_keeper is responsible only for the pointers not the objects
		for (size_t i = 0; i < this->count; ++i)
		{
			this->buffer[i] = other.buffer[i];
		}
	}

	ptr_keeper<T> & operator=(const ptr_keeper<T> & other)
	{
		if (this->count > other.get_count())
		{
			this->count = other.get_count();
			this->resize_rate = other.get_resize_rate();
			for (size_t i = 0; i < this->count; ++i)
			{
				this->buffer[i] = other.get(i);
			}
		}
		else
		{
			this->resize_rate = other.get_resize_rate();
			this->count = other.get_count();
			this->size = this->count * this->resize_rate;
			delete[] this->buffer;
			this->buffer = new T*[this->size];
			for (size_t i = 0; i < this->count; ++i)
			{
				this->buffer = other.get(i);
			}
		}
		return *this;
	}

	~ptr_keeper()
	{
		if (this->buffer)
		{
			delete[] this->buffer;
		}
	}

	size_t get_resize_rate() const
	{
		return this->resize_rate;
	}

	void set_resize_rate(size_t rr)
	{
		this->resize_rate = rr;
	}

	size_t get_count() const
	{
		return this->count;
	}

	size_t get_size() const
	{
		return this->size;
	}

	// Deletes all elements that keeped pointers are pointing to.
	void delete_elements()
	{
		for (size_t i = 0; i < this->count; ++i)
		{
			delete this->buffer[i];
		}
		this->count = 0;
	}

	// Keeps a pointer.
	void keep(T * element)
	{
		if (this->count == this->size)
		{
			this->expand();
		}
		this->buffer[this->count] = element;
		++this->count;
	}

	// Gives a pointer at a position <at>
	T * give(size_t at) const
	{
		if (at >= this->get_count())
		{
			throw std::out_of_range("Index provided for ptr_keeper::give() is out of range");
		}
		return this->buffer[at];
	}


	// Releases a pointer at a position <at>. No longer keeps it.
	void give_up(size_t at)
	{
		if (at >= this->get_count())
		{
			throw std::out_of_range("Index provided for ptr_keeper::give_up() is out of range");
		}
		for (size_t i = at; i < this->count - 1; ++i)
		{
			this->buffer[i] = this->buffer[i + 1];
		}
		--count;
	}
};

