#include "Line.h"
#include <string>
#include <iostream>

Line::Line()
{
	this->size = 0;
	this->isLoaded = false;
}

Line::Line(const char * content)
{
	this->isLoaded = false;
	this->load(content);
}

Line::Line(const Line & l)
{
	this->size = l.size;
	for (int i = 0; i < this->size; i++)
	{
		this->buffer[i] = l.getChar(i);
	}
}

Line::~Line()
{
	delete[] this->buffer;
}

bool Line::getIsLoaded() const
{
	return this->isLoaded;
}

void Line::load(const char * content)
{
	if (!this->isLoaded)
	{
		this->size = strlen(content) + 1;
		this->buffer = new char[size];
		strcpy_s(buffer, size, content);
		this->buffer[this->size - 1] = '\0';
		this->isLoaded = true;
	}
}

bool Line::setChar(int at, char ch)
{
	if (at < this->size - 1 && at >= 0)
	{
		this->buffer[at] = ch;
		return true;
	}
	return false;
}

char Line::getChar(int at) const
{
	return this->buffer[at];
}

int Line::getSize() const
{
	return this->size;
}

int Line::getLength() const
{
	return this->size - 1;
}

void Line::setNewContent(char * newbuffer, int size)
{
	if (newbuffer == nullptr)
	{
		return;
	}

	delete[] this->buffer;
	this->buffer = newbuffer;
	this->size = size;
}

void Line::print()
{
	for (int i = 0; i < this->size; i++)
	{
		std::cout << buffer[i];
	}
	std::cout << std::endl;
}
