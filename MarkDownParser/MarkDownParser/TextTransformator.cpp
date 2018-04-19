#include "TextTransformator.h"
#include <fstream>
#include <iostream>

TextTransformator::TextTransformator()
{
}

TextTransformator::~TextTransformator()
{
}

bool TextTransformator::load(const char * path)
{
	std::ifstream in;
	in.open(path);
	if (!in)
	{
		in.close();
		return false;
	}

	char buffer[1025]; // max 1024 symbols + '\0'
	while (true)
	{
		in.getline(buffer, 1025);
		if (in.eof())
		{
			break;
		}
		this->parser.addLine(buffer);
	}
	this->setPath(path);
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
	return false;
}

bool TextTransformator::save()
{
	std::ofstream file;
	file.open(fileName);
	if (!file)
	{
		file.close();
		return false;
	}

	char * line = this->parser.pop();
	while (line != nullptr)
	{
		file << line << std::endl;
		delete[] line;
		line = this->parser.pop();
	}
	file.close();
	return true;
}

bool TextTransformator::setPath(const char * p)
{
	int len = strlen(p);
	if (len >= 200)
	{
		return false;
	}
	for (int i = 0; i < len; i++)
	{
		path[i] = p[i];
	}
	path[len] = '\0';
	for (int i = 0; i < len - 4; i++)
	{
		fileName[i] = path[i];
	}
	fileName[len - 4] = '.';
	fileName[len - 3] = 'm';
	fileName[len - 2] = 'd';
	fileName[len - 1] = '\0';
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
