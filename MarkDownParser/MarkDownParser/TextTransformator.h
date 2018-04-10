#pragma once
#include "MDParser.h"
#include "Command.h"

class TextTransformator
{
public:
	TextTransformator();
	~TextTransformator();

	bool load(const char * path);

	bool execCommand(char * command);

	bool save();
private:
	char path[200];
	char fileName[200];
	bool setPath(const char*p);
	MDParser parser;

	bool exec(Command c);
};

