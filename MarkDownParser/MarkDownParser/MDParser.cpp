#include "MDParser.h"

MDParser::MDParser()
{
}

MDParser::~MDParser()
{

}

bool MDParser::makeHeader(int at)
{
	if (at <= 0)
	{
		return false;
	}
	Line * forparsing = this->file.getLine(at - 1);
	int size = forparsing->getSize();
	char * heading = new char[size + 2];
	heading[0] = '#';
	heading[1] = ' ';
	for (int i = 0; i < size; i++)
	{
		heading[i + 2] = forparsing->getChar(i);
	}
	heading[size + 2] = '\0';
	forparsing->setNewContent(heading, size + 1);
	return true;
}

bool MDParser::makeItalic(int at, int from, int to)
{
	if (from <= 0 || to <= 0)
	{
		return false;
	}
	// args are 0 based so we pass at and from  -1 but when we code a for loop we have '<' as a condition, not '<=', so we pass to (in otherwords to is exclusive)
	return this->setEmphasis(at - 1, from - 1, to, "*", 1);
	return false;
}

bool MDParser::setEmphasis(int at, int from, int to, const char * emp, int empSize)
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
	int newsize = size + 2 * empSize; // old size + 2 * emphasis characters
	char * newcontent = new char[newsize];

	int fromindex = 0, toindex = 0;
	for (int i = 0; i <= from; i++)
	{
		fromindex = line->getNextWordIndex(); // index of ' '
	}
	for (int i = from; i < to; i++)
	{
		toindex = line->getNextWordIndex(); // index of ' '
	}
	line->resetWordIndex();
	int index = 0;
	for (int i = 0; i < fromindex + 1 && fromindex != 0; i++)
	{
		newcontent[index] = line->getChar(i);
		++index;
	}
	for (int i = 0; i < empSize; i++)
	{
		newcontent[index++] = emp[i];
	}
	if (fromindex == 0)
	{
		fromindex = -1;
	}
	for (int i = fromindex + 1; i < toindex; i++)
	{
		newcontent[index++] = line->getChar(i);
	}
	for (int i = 0; i < empSize; i++)
	{
		newcontent[index++] = emp[i];
	}
	for (int i = toindex; i < line->getLength(); i++)
	{
		newcontent[index++] = line->getChar(i);
	}
	newcontent[index] = '\0';
	line->setNewContent(newcontent, newsize);
	return true;
}

bool MDParser::makeBold(int at, int from, int to)
{
	if (from <= 0 || to <= 0)
	{
		return false;
	}
	return this->setEmphasis(at - 1, from - 1, to, "**", 2);
}

bool MDParser::makeCombine(int at, int from, int to)
{
	if (from <= 0)
	{
		return false;
	}
	if (to <= 0)
	{
		return false;
	}
	return this->setEmphasis(at - 1, from - 1, to, "***", 3);
}

bool MDParser::addLine(char * content)
{
	this->file.addLine(content);
	return true;
}

bool MDParser::removeLine(int at)
{
	// input is 1 based so we pass 0 based
	this->file.removeLine(at - 1);
	return true;
}

char * MDParser::pop()
{
	return this->file.pop();
}
