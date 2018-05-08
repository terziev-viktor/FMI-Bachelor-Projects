#include <iostream>
#include "Moderator.h"



Moderator::Moderator()
{
}


Moderator::~Moderator()
{
}

void Moderator::remove()
{
	std::cout << "Moderator remove" << std::endl;
}

void Moderator::block()
{
	std::cout << "Moderator block" << std::endl;
}

void Moderator::unblock()
{
	std::cout << "Moderator unblock" << std::endl;
}
