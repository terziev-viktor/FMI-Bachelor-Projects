#include "Admin.h"

using namespace fmi::users;

Admin * fmi::users::Admin::getInstancePointer(const char * nickname, unsigned short age, unsigned int id)
{
	static Admin * admin = new Admin(nickname, age, id);
	return admin;
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
