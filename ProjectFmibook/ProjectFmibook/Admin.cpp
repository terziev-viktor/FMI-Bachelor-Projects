#include "Admin.h"

using namespace fmi::users;

Admin * fmi::users::Admin::getInstancePointer(const char * nickname, unsigned short age, unsigned int id)
{
	static Admin * admin = new Admin(nickname, age, id);
	return admin;
}

void fmi::users::Admin::addUser(User * userToAdd, List<User> & users)
{
	users.add(userToAdd);
}

void fmi::users::Admin::removeUser(unsigned int id, List<User>& users)
{
	unsigned int count = users.count();
	for (unsigned int i = 0; i < count; i++)
	{
		User * userToRemove = users.getAt(i);
		if (userToRemove->getId() == id)
		{
			if (userToRemove == this)
			{
				throw "The Admin can not remove himself";
			}
			users.removeAt(i);
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
