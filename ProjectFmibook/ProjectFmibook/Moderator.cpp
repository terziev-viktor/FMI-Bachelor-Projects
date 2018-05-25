#include "Moderator.h"

using fmi::users::Moderator;

unsigned int Moderator::OBJ_COUNT = 0;

Moderator::Moderator(const char * nickname, unsigned short age, unsigned int id)
	:BasicUser(nickname, age, id)
{
	OBJ_COUNT++;
}


Moderator::~Moderator()
{
	OBJ_COUNT--;
}

void fmi::users::Moderator::block(User * user)
{
	user->blocked = true;
}

void fmi::users::Moderator::unblock(User * user)
{
	user->blocked = false;
}

unsigned int fmi::users::Moderator::getObjectsCount()
{
	return Moderator::OBJ_COUNT;
}

void fmi::users::Moderator::removePost(unsigned int id, List<Post>& posts)
{
	unsigned int count = posts.count();
	for (unsigned int i = 0; i < count; i++)
	{
		Post * p = posts.getAt(i);
		if (p->getId() == id)
		{
			posts.removeAt(i);
			return;
		}
	}
	throw std::invalid_argument("This user has no post with this id");
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
