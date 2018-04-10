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
	bool addLine(char * content);
	bool removeLine(int at);
	char * pop();
private:
	MDFile file;
};

