#pragma once
#include "List.hpp"

#include "Admin.h"
#include "Post.h"
#include "LinkPost.h"
#include "TextPost.h"
#include "ImgPost.h"
#include "Moderator.h"

#include <iostream>
#include <fstream>
namespace fmi
{
	using namespace tools;
	using namespace posts;
	using namespace users;
	
	const int COMMANDS_COUNT = 11;
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
		"change_nickname",
		"info" 
	};
	const char COMMAND_QUIT[] = "quit";
	const char POST_TYPE_IMAGE[] = "[image]";
	const char POST_TYPE_LINK[] = "[url]";
	const char POST_TYPE_TEXT[] = "[text]";

	// This class only shows how Users and Posts hierarchies work
	// It controls only method calls. Logic is in Users and Posts classes
	// This shows that I'm using currect design patters for the project
	// For simplicity, all methods throw const char*
	class Fmibook
	{
	public:
		Fmibook(const char * adminNickname, unsigned short adminAge);
		// If the user wants to exetend DynamicArray<t> or List<T> and use their class
		Fmibook(const char * adminNickname, unsigned short adminAge, DynamicArray<User> * users, DynamicArray<Post> * posts);
		~Fmibook();
		// Only the admin can do these---
		void addModerator(const char * actor, const char * nickname, unsigned short age);
		void addUser(const char * actor, const char * nickname, unsigned short age);
		void removeUser(const char * actor, const char * who);
		// ----------
		void block(const char * actor, const char * who);
		void unblock(const char * actor, const char * who);
		void postImage(const char * actor, const char * content);
		void postLink(const char * actor, const char * url, const char * desctiption);
		void postText(const char * actor, const char * content);
		void removePost(const char * actor, unsigned int id);
		void viewPost(const char * actor, unsigned int id);
		void viewAllPosts(const char * actor, const char * ofwho);
		void changeNickname(const char * actor, const char * newNickname);
		void info();
	private:
		DynamicArray<User> * users;
		DynamicArray<Post> * posts;
		unsigned int nextId;
		User * getUserByNickname(const char * nickname);
	};

}

