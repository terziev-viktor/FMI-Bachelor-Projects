#include "Line.h"
#include <string>
#include <iostream>

Line::Line()
{
	this->size = 0;
	this->isLoaded = false;
	this->resetWordIndex();
}

Line::Line(const char * content)
{
	this->isLoaded = false;
	this->load(content);
}

Line::~Line()
{
	if (this->isLoaded)
	{
		delete[] this->buffer;
	}
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
		this->buffer = new char[this->size];
		for (int i = 0; i < this->size - 1; i++)
		{
			this->buffer[i] = content[i];
		}
		this->buffer[this->size - 1] = '\0';
		this->isLoaded = true;
		this->resetWordIndex();
	}
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

bool Line::setNewContent(char * newbuffer, int size)
{
	if (!this->isLoaded)
	{
		return false;
	}
	delete[] this->buffer;
	this->buffer = newbuffer;
	this->size = size;
	this->resetWordIndex();
	return true;
}

int Line::getNextWordIndex()
{
	int index = this->wordindex;
	while (wordindex < this->getLength() && this->buffer[wordindex] == ' ')
	{
		wordindex++;
	}
	while (wordindex < this->getLength() && this->buffer[wordindex] != ' ')
	{
		wordindex++;
	}
	return index;
}

void Line::resetWordIndex()
{
	if (this->isHeading())
	{
		this->wordindex = 1;
	}
	else
	{
		this->wordindex = 0;
	}
}

bool Line::isHeading()
{
	if (this->isLoaded)
	{
		return this->buffer[0] == '#';
	}
	return false;
}

void Line::print()
{
	for (int i = 0; i < this->size; i++)
	{
		std::cout << buffer[i];
	}
	std::cout << std::endl;
}
