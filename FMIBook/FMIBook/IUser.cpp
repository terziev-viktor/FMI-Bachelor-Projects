#include "IUser.h"



IUser::IUser()
{
}


IUser::~IUser()
{
}

char * IUser::getNickname()
{
	return this->nickname;
}

unsigned short IUser::getAge()
{
	return this->age;
}

bool IUser::isBlocked()
{
	return false;
}
