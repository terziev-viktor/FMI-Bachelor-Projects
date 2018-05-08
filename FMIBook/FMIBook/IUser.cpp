#include <iostream>
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

void IUser::setNickname(char * value)
{
	strcpy_s(this->nickname, 100, value);
}

void IUser::setAge(unsigned short age)
{
	this->age = age;
}
