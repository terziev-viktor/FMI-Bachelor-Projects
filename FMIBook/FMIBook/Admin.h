#include "Moderator.h"

#pragma once
class Admin : public Moderator
{
public:
	Admin();
	~Admin();
	void block();
	void unblock();
	void addUser();
	void removeUser();
};

