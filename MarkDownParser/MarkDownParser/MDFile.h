#include "Line.h"

#pragma once
class MDFile
{
public:
	MDFile();
	~MDFile();

	Line getLine(int at) const;
	Line * getAllLines() const;
	void addLine(Line);
private:
	Line * lines;
	int size;
	int index;
};

