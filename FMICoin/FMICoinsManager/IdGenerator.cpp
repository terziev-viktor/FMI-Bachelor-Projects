#include "IdGenerator.h"
#include <iostream>

unsigned int GenerateId()
{
	unsigned int id = 0;
	for (int i = 0; i < 200; i++)
	{
		int bit = rand() % 33;
		id = id ^ (1 << bit);
	}
	return id;
}