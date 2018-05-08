#include "User.h"

#pragma once
class Moderator : public User
{
public:
	Moderator();
	~Moderator();
	virtual void remove();
	virtual void block();
	virtual void unblock();
};

