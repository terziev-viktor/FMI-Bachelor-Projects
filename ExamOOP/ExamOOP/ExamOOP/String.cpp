#include "String.h"
#include "String.h"
#include <string.h>
#include <stdexcept>
#include <iostream>

String::String() :length(0), capacity(1), value(nullptr)
{
	this->value = new char[this->capacity];
	this->value[0] = '\0';
}

String::String(unsigned int length) :length(0), capacity(length + 1), value(nullptr)
{
	this->value = new char[this->capacity];
	this->value[0] = '\0';
}

String::String(const char * value)
{
	this->length = std::strlen(value);
	this->capacity = this->length + 1;
	this->value = new char[this->capacity];
	strcpy_s(this->value, this->capacity, value);
}

String::String(const String & other)
{
	this->length = other.get_length();
	this->capacity = this->length + 1;
	this->value = new char[this->capacity];
	strcpy_s(this->value, this->capacity, other.get_as_char_array());
}


String::~String()
{
	delete[] this->value;
}

unsigned int String::get_length() const
{
	return this->length;
}

unsigned int String::get_capacity() const
{
	return this->capacity;
}

String String::get_to_lower_case() const
{
	String result(this->get_length());
	for (size_t i = 0; i < this->get_length(); i++)
	{
		char c = this->value[i];
		if (c >= 'A' && c <= 'Z')
		{
			result += c + 32;
		}
		else
		{
			result += c;
		}
	}
	return result;
}

void String::set_capacity(unsigned int length)
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

const char * String::get_as_char_array() const
{
	return this->value;
}

String String::substring(unsigned int from, unsigned int to) const
{
	if (to < from)
	{
		throw std::logic_error("Cannot create a subString from staring position greater than ending position");
	}
	unsigned int capacity = to - from;
	String result(capacity);
	for (unsigned int i = from; i < to; i++)
	{
		result += (*this)[i];
	}
	return result;
}

int String::find_first_of(const String & c, unsigned int start_from) const
{
	int len = c.get_length();
	for (unsigned int i = start_from; i < this->get_length(); ++i)
	{
		for (int j = 0; j < len; ++j)
		{
			if (c[j] == this->value[i])
			{
				return i;
			}
		}
	}
	return -1;
}

bool String::contains(const char ch) const
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

Vector<String> String::split(const String & delim) const
{
	Vector<String> result;
	int from = 0;
	int to = 0;
	while (to < this->get_length())
	{
		to = this->find_first_of(delim, from);
		if (to < 0)
		{
			to = this->get_length();
		}
		result.add(this->substring(from, to));
		from = to + 1;
	}

	return result;
}

Vector<String> String::extract(const String & symbols) const
{
	Vector<String> result;
	int from = 0;
	int to = 1;
	while(to < this->get_length())
	{
		while (to < this->get_length() && symbols.contains(this->value[to]))
		{
			++to;
		}
		result.add(this->substring(from, to));
		while (to < this->get_length() && !symbols.contains(this->value[to]))
		{
			++to;
		}
		from = to;
		to = from + 1;
	}
	return result;
}

bool String::ends_with(const String & something) const
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

bool String::starts_with(const char ch) const
{
	return this->value[0] == ch;
}

bool String::to_int(int & out) const
{
	int result = 0;
	int multiplier = 1;
	size_t len = this->get_length();
	if (len == 0)
	{
		return false;
	}
	for (int i = len - 1; i > 0; --i)
	{
		if ((*this)[i] <= '9' && (*this)[i] >= '0')
		{
			result += ((*this)[i] - 48) * multiplier;
			multiplier *= 10;
		}
		else
		{
			return false;
		}
	}
	if ((*this)[0] == '-')
	{
		result *= -1;
	}
	else if ((*this)[0] <= '9' && (*this)[0] >= '0')
	{
		result += ((*this)[0] - 48) * multiplier;
		multiplier *= 10;
	}
	else
	{
		return false;
	}
	out = result;
	return true;
}

String String::to_String(int num)
{
	int num_cpy = num;
	int len = 0;
	while (num_cpy > 0)
	{
		++len;
		num_cpy /= 10;
	}
	String result(len);
	while (num > 0)
	{
		result += (char)((num % 10) + 48);
		num /= 10;
	}
	return result;
}

bool String::operator!=(const String & other) const
{
	return !(*this == other);
}

bool String::operator==(const String & other) const
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

String & String::operator+=(const String & other)
{
	unsigned int new_capacity = this->get_length() + other.get_length() + 1;
	char * newValue = new char[new_capacity];
	unsigned int index = 0;
	for (unsigned int i = 0; i < this->get_length(); ++i)
	{
		newValue[index] = this->value[i];
		index++;
	}

	for (unsigned int i = 0; i < other.get_length(); ++i)
	{
		newValue[index] = other[i];
		index++;
	}
	newValue[index] = '\0';

	delete[] this->value;
	this->value = newValue;
	this->capacity = new_capacity;
	this->length = new_capacity - 1;
	return *this;
}

// The idea here is to add one character very fast and efficient by initializing the object with greater capacity and then add char by char. Something like this:
// String str(big_capacity)
// for i=0 to big_capacity:
// str += some_char
String & String::operator+=(const char ch)
{
	if (this->capacity > this->length)
	{
		this->value[this->length] = ch;
		this->value[this->length + 1] = '\0';
		++this->length;
		return *this;
	}
	else
	{
		return *this += &ch;
	}

}

String & String::operator=(const String & other)
{
	this->length = other.get_length();
	this->capacity = other.get_capacity();
	delete[] this->value;
	this->value = new char[this->capacity];
	strcpy_s(this->value, this->capacity, other.get_as_char_array());
	return *this;
}

char String::operator[](unsigned int index) const
{
	if (index < 0 || index >= this->length)
	{
		throw std::out_of_range("String index out of range");
	}
	return this->value[index];
}

char & String::operator[](unsigned int index)
{
	if (index < 0 || index >= this->length)
	{
		throw std::out_of_range("String index out of range");
	}
	return this->value[index];
}

std::istream & operator>>(std::istream & in, String & obj)
{
	obj = "";
	char buffer[100];
	short i = 0;
	char c;
	in.get(c);
	while (c != ' ' && c != '\n' && c != '\t')
	{
		buffer[i++] = c;
		in.get(c);
		if (i == 99)
		{
			buffer[i] = '\0';
			obj += buffer;
			i = 0;
		}
	}
	if (i > 0)
	{
		buffer[i] = '\0';
		obj += buffer;
	}
	return in;
}

String operator+(const String & a, const String & b)
{
	String c(a);
	c += b;
	return c;
}

std::ostream & operator<<(std::ostream & os, const String & obj)
{
	const char * value = obj.get_as_char_array();
	os << (value);
	return os;
}