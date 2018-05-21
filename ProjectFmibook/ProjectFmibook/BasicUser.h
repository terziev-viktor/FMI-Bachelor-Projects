#pragma once
#include "User.h"
namespace fmi
{
	namespace users
	{
		class BasicUser : public User
		{
		public:
			BasicUser(const char * nickname, unsigned short age, unsigned int id);
			~BasicUser();
			// void post();
			// deleting post by id. Only if he is the post owner;
			//virtual void removePost(unsigned int id, User * owner);

			// basic user can change only their nickname and only if the new one is not used by another user
		};
	}
}


