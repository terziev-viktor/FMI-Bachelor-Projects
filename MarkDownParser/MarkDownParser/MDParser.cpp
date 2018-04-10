#include "MDParser.h"

MDParser::MDParser()
{
}

MDParser::~MDParser()
{

}

bool MDParser::makeHeader(int at)
{
	Line * forparsing = this->file.getLine(at);
	int size = forparsing->getSize();
	char * heading = new char[size + 2];
	heading[0] = '#';
	for (int i = 0; i < size; i++)
	{
		heading[i + 1] = forparsing->getChar(i);
	}
	heading[size + 1] = '\0';
	forparsing->setNewContent(heading, size + 1);
	return true;
}

bool MDParser::makeItalic(int at, int from, int to)
{
	if (from >= to)
	{
		return false;
	}
	Line * line = this->file.getLine(at);
	int size = line->getSize();
	if (from >= size || to >= size)
	{
		return false;
	}
	int newsize = size + 2; // old size + 2x'_'
	char * newcontent = new char[newsize];
	int i;
	for (i = 0; i < from; i++)
	{
		newcontent[i] = line->getChar(i);
	}
	newcontent[i] = '_';
	int index = i + 1;
	for (i = from; i < to; i++)
	{
		newcontent[index] = line->getChar(i);
		index++;
	}
	newcontent[index] = '_';
	index++;
	for (i = to; i <= line->getLength(); i++)
	{
		newcontent[index] = line->getChar(i);
		index++;
	}
	newcontent[index] = '\0';
	line->setNewContent(newcontent, newsize);
	return true;
}

bool MDParser::makeBold(int at, int from, int to)
{
	if (from >= to)
	{
		return false;
	}
	Line * line = this->file.getLine(at);
	int length = line->getLength();
	if (from >= length || to >= length)
	{
		return false;
	}
	int newsize = line->getSize() + 4;
	char * newcontent = new char[newsize]; // '*'x4 for bold + '\0'
	int i;
	for (i = 0; i < from; i++)
	{
		newcontent[i] = line->getChar(i);
	}
	newcontent[i] = '*';
	int index = i + 1;
	newcontent[index] = '*';
	index += 1;
	for (i = from; i < to; i++)
	{
		newcontent[index] = line->getChar(i);
		index++;
	}
	newcontent[index] = '*';
	index++;
	newcontent[index] = '*';
	index++;
	for (i = to; i <= length; i++)
	{
		newcontent[index] = line->getChar(i);
		index++;
	}
	newcontent[index] = '\0';
	line->setNewContent(newcontent, newsize);
	return true;
}

bool MDParser::makeCombine(int at, int from, int to)
{
	if (from >= to)
	{
		return false;
	}
	Line * line = this->file.getLine(at);
	int size = line->getSize();
	if (from >= size || to >= size)
	{
		return false;
	}
	int newsize = size + 6;
	char * newcontent = new char[newsize]; // '*'x4 for bold + 2x'_' for italic
	int i;
	for (i = 0; i < from; i++)
	{
		newcontent[i] = line->getChar(i);
	}
	newcontent[i] = '*';
	int index = i + 1;
	newcontent[index] = '*';
	index++;
	newcontent[index] = '_';
	index++;
	for (i = from; i < to; i++)
	{
		newcontent[index] = line->getChar(i);
		index++;
	}
	newcontent[index] = '*';
	index++;
	newcontent[index] = '*';
	index++;
	newcontent[index] = '_';
	index++;
	for (i = to; i < line->getLength(); i++)
	{
		newcontent[index] = line->getChar(i);
		index++;
	}
	newcontent[index] = '\0';
	line->setNewContent(newcontent, newsize);
	return true;
}

bool MDParser::addLine(char * content)
{
	this->file.addLine(content);
	return true;
}

bool MDParser::removeLine(int at)
{
	this->file.removeLine(at);
	return true;
}

char * MDParser::pop()
{
	return this->file.pop();
}
