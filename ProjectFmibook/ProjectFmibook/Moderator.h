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

			static unsigned int getObjectsCount();

			void removePost(unsigned int id, List<Post> & posts);
			
		private:
			static unsigned int OBJ_COUNT;
		};
	}
}


