#include "User.h"

#include <string.h>
#include <stdexcept>

using fmi::users::User;

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

void fmi::users::User::addPost(fmi::posts::Post * post)
{
	this->posts.add(post);
}



bool fmi::users::User::isBlocked() const
{
	return this->blocked;
}

void fmi::users::User::setNickname(const char * newnickname)
{
	if (newnickname == NULL)
	{
		throw std::invalid_argument("Invalid new nickname for User");
	}
	strcpy_s(this->nickname, sizeof(this->nickname), newnickname);
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
