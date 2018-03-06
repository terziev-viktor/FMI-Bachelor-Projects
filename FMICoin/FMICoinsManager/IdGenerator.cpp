#include "IdGenerator.h"

unsigned int GenerateId()
{
	unsigned int id = 0;
	for (int i = 0; i < 100; i++)
	{
		int bit = rand() % 33;
		id = id ^ (1 << bit);
	}
	return id;
}