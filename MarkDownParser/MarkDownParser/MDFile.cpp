#include "MDFile.h"

MDFile::MDFile()
{
}

MDFile::~MDFile()
{

}

void MDFile::addLine(Line & l)
{
	this->lineq.addLine(l);
}

void MDFile::setLine(Line & l, int at)
{

}

Line * MDFile::getLine(int at)
{
	return this->lineq.getLine(at);
}
