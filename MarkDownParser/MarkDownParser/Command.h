#pragma once

const char MAKE_HEADING[] = "makeHeading";
const char MAKE_ITALIC[] = "makeItalic";
const char MAKE_BOLD[] = "makeBold";
const char MAKE_COMBINE[] = "makeCombine";
const char ADD_LINE[] = "addLine";
const char REMOVE[] = "remove";
const char EXIT[] = "exit";

enum CommandType
{
	MakeHeading = 0, // line number - Makes the line Header
	makeItalic, // line from to - makes words itallic
	makeBold, // line from to - makes words bold
	makeCombine, // line from to - makes words bold and itallic
	addLine, // addLine lineContent – adds line to the file
	remove, // lineNumber - removes a line by 
	exit // exit the program
};

class Command
{
public:
	Command(char * command);
	~Command();
	CommandType getType() const;
	int compare(const Command & other);
private:
	const char commandTypeAsString[7][20] = { "makeHeading", "makeItalic", "makeBold", "makeCombine", "addLine", "remove", "exit" };
	CommandType type;
};