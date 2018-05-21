#pragma once
#include "List.hpp"

#include "Admin.h"
#include "Post.h"
#include "LinkPost.h"
#include "TextPost.h"
#include "ImgPost.h"

namespace fmi
{
	using namespace tools;
	using namespace posts;
	using namespace users;
	
	const int COMMANDS_COUNT = 10;
	const char COMMANDS[COMMANDS_COUNT][21] = 
	{ 
		"add_moderator", 
		"add_user", 
		"remove_user", 
		"block", 
		"unblock", 
		"post", 
		"remove_post",
		"view_post", 
		"view_all_posts",
		"info" 
	};
	const char COMMAND_QUIT[] = "quit";
	const char POST_TYPE_IMAGE[] = "[image]";
	const char POST_TYPE_LINK[] = "[url]";
	const char POST_TYPE_TEXT[] = "[text]";

	class Fmibook
	{
	public:
		Fmibook(const char * adminNickname, unsigned short adminAge);
		~Fmibook();
		// Only the admin can do these---
		void addModerator(char * actor, char * nickname, unsigned short age);
		void addUser(char * actor, char * nickname, unsigned short age);
		void removeUser(char * actor, char * who);
		// ----------
		void block(char * actor, char * who);
		void unblock(char * actor, char * who);
		void postImage(char * actor, char * content);
		void postLink(char * actor, char * content);
		void postText(char * actor, char * content);
		void removePost(char * actor, unsigned int id);

	private:
		List<User> users;
		unsigned int nextId;

		User * getUserByNickname(char * nickname);
	};

}

