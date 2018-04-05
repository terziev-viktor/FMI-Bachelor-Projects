#pragma once

const int COMMANDS_COUNT = 8;
const int COMMAND_MAX_LEN = 20;
const char COMMANDS[COMMANDS_COUNT][COMMAND_MAX_LEN] = { "makeHeading", "makeItalic", "makeBold" , "makeCombine" , "addLine" , "remove" };
const char EXIT[] = "exit";

enum Command
{
	MakeHeading = 0, // line number - Makes the line Header
	MakeItalic, // line from to - makes words itallic
	MakeBold, // line from to - makes words bold
	MakeCombine, // line from to - makes words bold and itallic
	AddLine, // addLine lineContent – adds line to the file
	Remove // lineNumber - removes a line by
};
