#include "Moderator.h"

using fmi::users::Moderator;

Moderator::Moderator(const char * nickname, unsigned short age, unsigned int id)
	:BasicUser(nickname, age, id)
{
}


Moderator::~Moderator()
{
}

void fmi::users::Moderator::block(User * user)
{

}

void fmi::users::Moderator::unblock(User * user)
{
	user->blocked = false;
}

//void fmi::users::Moderator::removePost(unsigned int id, User * owner)
//{
//	if (owner == nullptr)
//	{
//		throw "Null pointer not allowed";
//	}
//
//	owner->removePost(id);
//}
