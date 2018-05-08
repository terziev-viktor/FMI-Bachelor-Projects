#include <iostream>
#include "Admin.h"

Admin::Admin()
{
}


Admin::~Admin()
{
}

void Admin::block()
{
	std::cout << "Admin block" << std::endl;
}

void Admin::unblock()
{
	std::cout << "Admin unblock" << std::endl;
}

void Admin::addUser()
{
	std::cout << "Admin add user" << std::endl;
}

void Admin::removeUser()
{
	std::cout << "Admin remove user" << std::endl;
}

void Admin::remove()
{
	std::cout << "Admin remove" << std::endl;
}
