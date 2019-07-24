#include <iostream>
#include <string>
#include <fstream>
#include "ConfigFile.h"

using namespace std;
int main()
{
	int actual_size = 300;
	string content(actual_size, 'c');
	string file_name = "MassTransit.cfg";
	
	ConfigFile file(eConfigFile_MTConfig);
	int sz = file.Read(file_name.c_str());

	file.SetPropertyForConfigFileMerge("SEND_FILE_ORDER", "69");
	file.Write();

	return 0;
}
