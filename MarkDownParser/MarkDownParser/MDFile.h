#include "Queue.h"
#pragma once
class MDFile
{
public:
	MDFile();
	~MDFile();
	void addLine(Line & l);
	void setLine(Line & l, int at);
	Line * getLine(int at);
private:
	Queue lineq;
};

