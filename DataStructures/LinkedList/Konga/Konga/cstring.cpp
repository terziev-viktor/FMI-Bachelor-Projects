#include "cstring.h"
#include <cstring>

cstring::cstring() :length(0), capacity(1), value(nullptr)
{
	this->value = new char[this->capacity];
	this->value[0] = '\0';
}

cstring::cstring(unsigned int length) :length(0), capacity(length + 1), value(nullptr)
{
	this->value = new char[this->capacity];
	this->value[0] = '\0';
}

cstring::cstring(const char * value)
{
	this->length = std::strlen(value);
	this->capacity = this->length + 1;
	this->value = new char[this->capacity];
	strcpy_s(this->value, this->capacity, value);
}

cstring::cstring(const cstring & other)
{
	this->length = other.get_length();
	this->capacity = this->length + 1;
	this->value = new char[this->capacity];
	strcpy_s(this->value, this->capacity, other.get_as_char_array());
}


cstring::~cstring()
{
	delete[] this->value;
}

unsigned int cstring::get_length() const
{
	return this->length;
}

unsigned int cstring::get_capacity() const
{
	return this->capacity;
}

void cstring::set_capacity(unsigned int length)
{
	if (!value)
	{
		this->value = new char[this->capacity];
		this->value[0] = '\0';
	}
	this->capacity = length + 1;
	char * buffer = new char[this->capacity];
	unsigned int min = this->get_length() < length ? this->get_length() : length;
	for (unsigned int i = 0; i <= min; i++)
	{
		buffer[i] = this->value[i];
	}
	delete[] this->value;
	this->value = buffer;
}

const char * cstring::get_as_char_array() const
{
	return this->value;
}

bool cstring::contains(const char ch) const
{
	for (unsigned int i = 0; i < this->get_length(); i++)
	{
		if ((*this)[i] == ch)
		{
			return true;
		}
	}
	return false;
}

bool cstring::ends_with(const cstring & something) const
{
	if (something.get_length() > this->get_length())
	{
		return false;
	}
	for (unsigned int i = 0; i < something.get_length(); i++)
	{
		if ((*this)[this->get_length() - 1 - i] != something[something.get_length() - 1 - i])
		{
			return false;
		}
	}
	return true;
}

bool cstring::starts_with(const char ch) const
{
	return this->value[0] == ch;
}

bool cstring::operator!=(const cstring & other) const
{
	return !(*this == other);
}

bool cstring::operator==(const cstring & other) const
{
	if (other.get_length() != this->get_length())
	{
		return false;
	}
	for (unsigned int i = 0; i < this->get_length(); i++)
	{
		if (other[i] != (*this)[i])
		{
			return false;
		}
	}
	return true;
}

cstring & cstring::operator=(const cstring & other)
{
	this->length = other.get_length();
	this->capacity = other.get_capacity();
	delete[] this->value;
	this->value = new char[this->capacity];
	strcpy_s(this->value, this->capacity, other.get_as_char_array());
	return *this;
}

char cstring::operator[](unsigned int index) const
{
	if (index < 0 || index >= this->length)
	{
		throw "cstring index out of range";
	}
	return this->value[index];
}

char & cstring::operator[](unsigned int index)
{
	if (index < 0 || index >= this->length)
	{
		throw "cstring index out of range";
	}
	return this->value[index];
}
