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

void fmi::users::BasicUser::removePost(unsigned int id, DynamicArray<Post> * posts)
{
	unsigned int count = posts->count();
	for (unsigned int i = 0; i < count; i++)
	{
		Post * p = posts->getAt(i);
		if (p->getId() == id)
		{
			if (p->getOwnerId() == this->id)
			{
				posts->removeAt(i);
				return;
			}
			else
			{
				throw "This user is not the owner of the post";
			}
		}
	}
	throw "This user has no post with this id";
}
