#include "User.h"

#include <string.h>
#include <stdexcept>

using fmi::users::User;
using fmi::tools::DynamicArray;

User::User(const char * nickname, unsigned short age, unsigned int id)
{
	this->setNickname(nickname);
	this->setAge(age);
	this->blocked = false;
	this->id = id;
}


User::~User()
{
}

User & fmi::users::User::operator=(const User & other)
{
	this->setNickname(other.getNickname());
	this->setAge(other.getAge());
	this->id = other.id;
	this->blocked = other.blocked;

	return *this;
}

void fmi::users::User::addPost(fmi::posts::Post * post, DynamicArray<Post> * posts)
{
	if (this->blocked)
	{
		throw "Blocked users can't post";
	}
	posts->add(post);
}

const bool fmi::users::User::isBlocked() const
{
	return this->blocked;
}

void fmi::users::User::setNickname(const char * newnickname)
{
	if (newnickname == NULL)
	{
		throw std::invalid_argument("Invalid new nickname for User");
	}
	strcpy_s(this->nickname, MAX_NICKNAME_LENGTH, newnickname);
}

const char * fmi::users::User::getNickname() const
{
	return this->nickname;
}

void fmi::users::User::setAge(unsigned short age)
{
	this->age = age;
}

const unsigned short fmi::users::User::getAge() const
{
	return this->age;
}

const unsigned int fmi::users::User::getId() const
{
	return this->id;
}

void User::changeNickname(const char * newNickName, DynamicArray<User> * users)
{
	unsigned int count = users->count();
	for (unsigned int i = 0; i < count; i++)
	{
		User * u = users->getAt(i);
		if (strcmp(u->getNickname(), newNickName) == 0)
		{
			throw "This nickname is already used";
		}
	}
	this->setNickname(newNickName);
}

