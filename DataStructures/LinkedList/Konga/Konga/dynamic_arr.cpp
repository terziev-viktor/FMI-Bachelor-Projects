#include "dynamic_arr.h"

void dynamic_arr::expand()
{
	this->size *= this->resize_rate;
	Konga * new_buffer = new Konga[this->size];
	for (size_t i = 0; i < this->count; ++i)
	{
		new_buffer[i] = this->buffer[i];
	}
	delete[] this->buffer;
	this->buffer = new_buffer;
}

dynamic_arr::dynamic_arr(size_t preserve_size)
{
	this->size = preserve_size;
	this->count = 0;
	this->buffer = new Konga[this->size];
}

dynamic_arr::~dynamic_arr()
{
	if (this->buffer)
	{
		delete[] this->buffer;
	}
}

dynamic_arr & dynamic_arr::push_back(Konga & value)
{
	if (this->count == this->size)
	{
		this->expand();
	}
	this->buffer[this->count++].swap_with(value);
	return *this;
}

dynamic_arr & dynamic_arr::remove(size_t at)
{
	if (at >= this->count)
	{
		throw std::out_of_range("Index out of range. Can not remove element that is out of bounds of the array");
	}

	for (size_t i = at; i < this->count - 1; ++i)
	{
		this->buffer[i].swap_with(this->buffer[i + 1]);
	}

	--this->count;

	return *this;
}
