#include "MDParser.h"
#include "Command.h"

#pragma once
class TextTransformator
{
public:
	TextTransformator();
	~TextTransformator();

	bool load(char * path);

	bool execCommand(char * command);
private:
	MDParser parser;
	bool exec(Command c);
};

