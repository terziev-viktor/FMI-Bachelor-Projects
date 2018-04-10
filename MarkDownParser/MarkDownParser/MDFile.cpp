#include "MDFile.h"

MDFile::MDFile()
{
}

MDFile::~MDFile()
{

}

void MDFile::addLine(char * content)
{
	Line * l = new Line();
	l->load(content);
	this->lineq.addLine(l);
}

void MDFile::removeLine(int at)
{
	this->lineq.removeAt(at);
}

Line * MDFile::getLine(int at)
{
	return this->lineq.getLine(at);
}

char * MDFile::pop()
{
	return this->lineq.pop();
}
