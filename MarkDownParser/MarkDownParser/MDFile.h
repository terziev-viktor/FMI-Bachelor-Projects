#include "Queue.h"
#pragma once
class MDFile
{
public:
	MDFile();
	~MDFile();
	void addLine(char * content);
	void removeLine(int at);
	Line * getLine(int at);
	char * pop();
private:
	Queue lineq;
};

