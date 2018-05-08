#include "IUser.h"

#pragma once
class User : public IUser
{
public:
	User();
	~User();
	void post();
	virtual void remove();
	void changeNickname();
};

