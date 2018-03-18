#pragma once
#include "IdGenerator.h"
#include <iostream>

bool SetInitId()
{
	std::ofstream out;
	out.open(ID, std::ios::binary);
	if (!out.good())
	{
		out.close();
		return false;
	}
	unsigned int id = 1;
	out.write(reinterpret_cast<char*>(&id), sizeof(unsigned int));
	out.close();
	return true;
}

bool GenerateId(unsigned & id)
{
	std::ifstream in;
	in.open(ID, std::ios::binary | std::ios::ate);
	if (!in.good())
	{
		in.close();
		return false;
	}
	long long size = in.tellg();
	if (!size)
	{
		in.close();
		bool success = SetInitId();
		if (!success)
		{
			return false;
		}
		in.open(ID, std::ios::binary);
	}
	in.seekg(0, std::ios::beg);
	
	in.read(reinterpret_cast<char*>(&id), sizeof(unsigned));
	in.close();
	std::ofstream out;
	out.open(ID, std::ios::binary);
	unsigned int nextId = id + 1;
	out.write(reinterpret_cast<char*>(&nextId), sizeof(unsigned));
	out.close();
	
	return true;
}