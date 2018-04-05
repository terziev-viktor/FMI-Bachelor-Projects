#include "Line.h"
#include <string>
#include <iostream>

Line::Line()
{
	this->size = 0;
	this->index = 0;
	this->isLoaded = false;
}

Line::Line(char * content)
{
	this->buffer = new char[this->INIT_SIZE];
	this->size = this->INIT_SIZE;
	this->index = 0;
	strcpy_s(buffer, sizeof(char) * this->INIT_SIZE, content);
	this->isLoaded = true;
}

Line::Line(const Line & l)
{
	this->index = l.index;
	this->size = l.size;
	for (int i = 0; i < index; i++)
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
	strcpy_s(buffer, sizeof(char) * this->INIT_SIZE, content);
	this->isLoaded = true;
}

bool Line::setChar(int at, char ch)
{
	if (at < this->index && at >= 0)
	{
		this->buffer[at] = ch;
		return true;
	}
	return false;
}

void Line::addChar(char ch)
{
	if (this->index == size)
	{
		this->expand(1);
	}
	this->buffer[this->index] = ch;
	++this->index;
}

char Line::getChar(int at) const
{
	return this->buffer[at];
}

void Line::cpy(char * dest)
{
	for (int i = 0; i < this->index; i++)
	{
		dest[i] = this->buffer[i];
	}
}

void Line::expand(int n)
{
	char * expanded = new char[size + n];
	this->size += n;
	this->cpy(expanded);
	delete[] this->buffer;
	this->buffer = expanded;
}

void Line::print()
{
	for (int i = 0; i < this->index; i++)
	{
		std::cout << buffer[i];
	}
	std::cout << std::endl;
}
