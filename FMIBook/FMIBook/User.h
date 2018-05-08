#include "IUser.h"

#pragma once
class User : public IUser
{
public:
	User();
	~User();
	void post();
	void remove();
	void changeNickname();
};

