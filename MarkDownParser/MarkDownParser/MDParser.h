#pragma once
#include "MDFile.h"

class MDParser
{
public:
	MDParser();
	~MDParser();
	bool makeHeader(int at);
	// <from> and <to> are 1 based. they represent the index of the word 
	bool makeItalic(int line, int from, int to);
	// <from> and <to> are 1 based. they represent the index of the word 
	bool makeBold(int line, int from, int to);
	// <from> and <to> are 1 based. they represent the index of the word 
	bool makeCombine(int line, int from, int to);
	bool addLine(char * content);
	bool removeLine(int at);
	// Deletes the root. Returns a pointer to a dynamically allocated array of chars with the content of the root.
	// User should delete the memory.
	char * pop();
private:
	MDFile file;
	// args are 0 based! from, to are word indecies
	bool setEmphasis(int at, int from, int to, const char*emphasis, int sizeOfEmphasis);
};

