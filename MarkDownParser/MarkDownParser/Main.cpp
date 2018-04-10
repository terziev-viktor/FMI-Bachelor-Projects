#include <iostream>
#include "TextTransformator.h"
using namespace std;

void Run(TextTransformator & transformator)
{
	char command[100];
	cin >> command;
	while (strcmp(command, EXIT) != 0)
	{
		bool success = transformator.execCommand(command);
		if (!success)
		{
			cout << "Could not execute command: " << command << endl;
		}
		cin >> command;
	}
}

int main()
{
	char path[1024];
	cin >> path;
	TextTransformator transformator;
	bool loaded = transformator.load(path);
	while (!loaded)
	{
		cout << "Could not load file at:" << path << endl;
		cin >> path;
		loaded = transformator.load(path);
	}

	Run(transformator);

	transformator.save();
	return 0;
}