#include <iostream>
#include "User.h"



User::User()
{
}


User::~User()
{
}

void User::post()
{
	std::cout << "User post" << std::endl;
}

void User::remove()
{
	std::cout << "User remove" << std::endl;
}

void User::changeNickname()
{
	std::cout << "User change nick" << std::endl;
}
