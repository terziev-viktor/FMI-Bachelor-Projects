#pragma once
#include "Post.h"
#include "List.hpp"

namespace fmi
{
	namespace users
	{
		class User
		{
		public:
			User(const char * nickname, unsigned short age, unsigned int id);
			// virtual destrcutor for working with pointers
			virtual ~User();
			bool isBlocked() const;
			void setNickname(const char *);
			const char * getNickname() const;
			void setAge(unsigned short age);
			const unsigned short getAge() const;
			User & operator=(const User & other);

			// the Admin and moderators should be able to edit .blocked of other objects
			friend class Moderator;
			friend class Admin;
			
			void addPost(fmi::posts::Post * post);
			//virtual void removePost(unsigned int id) = 0;

		protected:
			bool blocked;
			unsigned int id;
			tools::List<posts::Post> posts;
		private:
			static const unsigned int MAX_NICKNAME_LENGTH = 100;

			char nickname[MAX_NICKNAME_LENGTH];
			unsigned short age;
			
		};
	}
}



