#include "MDParser.h"

MDParser::MDParser()
{
}

MDParser::~MDParser()
{
}

bool MDParser::makeHeader(int at)
{

}

bool MDParser::makeItalic(int line, int from, int to)
{
	return false;
}

bool MDParser::makeBold(int line, int from, int to)
{
	return false;
}

bool MDParser::makeCombine(int line, int from, int to)
{
	return false;
}

bool MDParser::addLine(const char * content)
{
	return false;
}

bool MDParser::removeLine(int at)
{
	return false;
}
