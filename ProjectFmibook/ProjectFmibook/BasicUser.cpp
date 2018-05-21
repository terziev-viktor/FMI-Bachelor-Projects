#include "BasicUser.h"

using fmi::users::BasicUser;
using fmi::posts::Post;

BasicUser::BasicUser(const char * nickname, unsigned short age, unsigned int id)
	:User(nickname, age, id)
{

}


BasicUser::~BasicUser()
{
}

//void fmi::users::BasicUser::removePost(unsigned int id)
//{
//	unsigned int count = this->posts.count();
//	Post * p;
//	for (int i = 0; i < count; i++)
//	{
//		p = this->posts.getAt(i);
//		if (p->getId() == id)
//		{
//			this->posts.removeAt(i);
//			return;
//		}
//	}
//	throw "This user has no such post";
//}