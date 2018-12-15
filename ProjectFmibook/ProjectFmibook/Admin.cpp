#include "Admin.h"
#include <cstring>

using namespace fmi::users;

Admin * fmi::users::Admin::getInstancePointer(const char * nickname, unsigned short age, unsigned int id)
{
	static Admin * admin = new Admin(nickname, age, id);
	return admin;
}

void fmi::users::Admin::addUser(User * userToAdd, DynamicArray<User> * users)
{
	unsigned int count = users->count();
	for (unsigned int i = 0; i < count; i++)
	{
		User * u = users->getAt(i);
		if (strcmp(u->getNickname(), userToAdd->getNickname()) == 0)
		{
			throw "Username taken";
		}
	}
	users->add(userToAdd);
}

void fmi::users::Admin::removeUser(unsigned int id, DynamicArray<User> * users, DynamicArray<Post> * posts)
{
	unsigned int count = users->count();
	for (unsigned int i = 0; i < count; i++)
	{
		User * userToRemove = users->getAt(i);
		if (userToRemove->getId() == id)
		{
			if (userToRemove == this)
			{
				throw "The Admin cannot remove himself";
			}
			users->removeAt(i);
			for (unsigned int i = 0; i < posts->count(); i++)
			{
				Post * p = posts->getAt(i);
				if (p->getOwnerId() == id)
				{
					posts->removeAt(i);
					--i; // the next post will be on index = index of removed post
				}
			}
			return;
		}
	}
}

Admin::Admin(const char * nickname, unsigned short age, unsigned int id)
	:Moderator(nickname, age, id)
{

}


Admin::~Admin()
{
}

void fmi::users::Admin::operator=(const Admin &)
{
	throw "Not implemented";
}
