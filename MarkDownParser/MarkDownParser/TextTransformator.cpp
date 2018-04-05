#include "TextTransformator.h"
#include <fstream>
#include <iostream>

TextTransformator::TextTransformator()
{
}

TextTransformator::~TextTransformator()
{
}

bool TextTransformator::load(char * path)
{
	std::ifstream in;
	in.open(path);
	if (!in)
	{
		in.close();
		std::ofstream out;
		out.open(path);
		out.close();
		in.open(path);
	}
	if (!in)
	{
		in.close();
		return false;
	}

	char buffer[1024];
	while (true)
	{
		in.getline(buffer, 1024);
		if (in.eof())
		{
			break;
		}
		this->parser.addLine(buffer);
	}
	return true;
}

bool TextTransformator::execCommand(char * command)
{
	for (int i = 0; i < COMMANDS_COUNT; i++)
	{
		if (strcmp(command, COMMANDS[i]) == 0)
		{
			return this->exec((Command)i);
		}
	}
	return true;
}

bool TextTransformator::exec(Command c)
{
	switch (c)
	{
	case MakeHeading:
	{
		int at;
		std::cin >> at;
		return this->parser.makeHeader(at);
	}
	case MakeItalic:
	{
		int line, from, to;
		std::cin >> line >> from >> to;
		return this->parser.makeItalic(line, from, to);
	}
	case MakeBold:
	{
		int line, from, to;
		std::cin >> line >> from >> to;
		return this->parser.makeBold(line, from, to);
	}
	case MakeCombine:
	{
		int line, from, to;
		std::cin >> line >> from >> to;
		return this->parser.makeCombine(line, from, to);
	}
	case AddLine:
	{
		char content[1024];
		std::cin.getline(content, 1024);
		return this->parser.addLine(content);
	}
	case Remove:
	{
		int at;
		std::cin >> at;
		return this->parser.removeLine(at);
	}
	default:
		break;
	}
	return false;
}
