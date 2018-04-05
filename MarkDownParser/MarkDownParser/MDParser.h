#pragma once
#include "MDFile.h"

class MDParser
{
public:
	MDParser();
	~MDParser();
	bool makeHeader(int at);
	bool makeItalic(int line, int from, int to);
	bool makeBold(int line, int from, int to);
	bool makeCombine(int line, int from, int to);
	bool addLine(const char * content);
	bool removeLine(int at);
private:
	MDFile file;
};

