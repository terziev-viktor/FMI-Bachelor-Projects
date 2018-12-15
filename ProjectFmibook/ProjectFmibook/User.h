#pragma once
#include "Post.h"
#include "DynamicArray.hpp"

namespace fmi
{
	namespace users
	{
		using fmi::tools::DynamicArray;
		using fmi::posts::Post;

		class User
		{
		public:
			User(const char * nickname, unsigned short age, unsigned int id);
			// virtual destrcutor for working with pointers
			virtual ~User();
			static const unsigned int MAX_NICKNAME_LENGTH = 100;

			void setAge(unsigned short age);

			const bool isBlocked() const;
			const char * getNickname() const;
			const unsigned short getAge() const;
			const unsigned int getId() const;
			User & operator=(const User & other);

			// the Admin and moderators should be able to edit .blocked of other objects
			friend class Moderator;
			friend class Admin;

			void changeNickname(const char * newNickName, DynamicArray<User> * users);
			void addPost(Post * post, DynamicArray<Post> * posts);
			virtual void removePost(unsigned int id, DynamicArray<Post> * posts) = 0;

		protected:
			bool blocked;
			unsigned int id;

			void setNickname(const char *);
		private:

			char nickname[MAX_NICKNAME_LENGTH];
			unsigned short age;
			
		};
	}
}



