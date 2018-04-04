#pragma once
#include "MDFile.h"

class MDParser
{
public:
	MDParser();
	~MDParser();
	bool load(char * path);
	void makeHeader(int at);
	void makeItalic(int line, int from, int to);
	void makeBold(int line, int from, int to);
	void makeCombine(int line, int from, int to);
	void addLine(char * content);
	void removeLine(int at);
private:
	MDFile file;
};

