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
			virtual void removePost(unsigned int id, DynamicArray<Post> * posts);
			
			
			// basic user can change only their nickname and only if the new one is not used by another user
		};
	}
}


