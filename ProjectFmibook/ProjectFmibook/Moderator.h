#pragma once
#include "BasicUser.h"

namespace fmi
{
	namespace users
	{
		class Moderator : public BasicUser
		{
		public:
			Moderator(const char * nickname, unsigned short age, unsigned int id);
			~Moderator();
			void block(User * user);
			void unblock(User * user);
			//virtual void removePost(unsigned int id, User * owner);
		};
	}
}


