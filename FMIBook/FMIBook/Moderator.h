#include "User.h"

#pragma once
class Moderator : public User
{
public:
	Moderator();
	~Moderator();
	void remove();
	virtual void block();
	virtual void unblock();
};

