#pragma once
#include <stdexcept>
#include "doubly_linked_list.hpp"

// Dynamic array of kongas.
// No memory copy of objects in the kongas. Only the pointers are being swappend using the swap_with() method
// No copy c-tor and operator= because I'm not using them
class dynamic_arr
{
private:
	size_t size;
	size_t count;
	Konga * buffer;
	size_t resize_rate = 2;

	void expand();

public:
	dynamic_arr(size_t preserve_size = 15);

	~dynamic_arr();

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

	Konga & operator[](size_t index)
	{
		return this->buffer[index];
	}

	const Konga & operator[](size_t index) const
	{
		return this->buffer[index];
	}

	dynamic_arr & push_back(Konga & value);

	dynamic_arr & remove_back()
	{
		return this->remove(this->count - 1);
	}

	dynamic_arr & remove(size_t at);
};

