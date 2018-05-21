#include "Fmibook.h"
#include "User.h"
#include "Post.h"

#include <stdexcept>

using fmi::Fmibook;
using fmi::users::User;
using fmi::posts::Post;

fmi::Fmibook::Fmibook(const char * adminNickname, unsigned short adminAge)
{
	this->nextId = 1;
	this->users.add(Admin::getInstancePointer(adminNickname, adminAge, this->nextId));
	++this->nextId;
}

Fmibook::~Fmibook()
{
}

void fmi::Fmibook::addModerator(char * actor, char * nickname, unsigned short age)
{
	User * p = this->getUserByNickname(actor);
	if (p == nullptr)
	{
		throw std::invalid_argument("User does not exist");
	}
	if (dynamic_cast<Admin*>(p))
	{
		this->users.add(new Moderator(nickname, age, nextId));
		++nextId;
	}
	else
	{
		throw std::invalid_argument("Only the admin can add a new moderator");
	}
}

void fmi::Fmibook::addUser(char * actor, char * nickname, unsigned short age)
{
	User * p = this->getUserByNickname(actor);
	if (p == nullptr)
	{
		throw std::invalid_argument("User does not exist");
	}
	if (dynamic_cast<Admin*>(p))
	{
		this->users.add(new BasicUser(nickname, age, nextId));
		++nextId;
	}
	else
	{
		throw std::invalid_argument("Only the admin can add a new user");
	}
}

void fmi::Fmibook::removeUser(char * actor, char * nickname)
{
	throw "Not implemented .removeUser of Fmibook";
}

void fmi::Fmibook::block(char * actor, char * nickname)
{
	User * p = getUserByNickname(actor);
	User * user = getUserByNickname(nickname);
	if (user == nullptr)
	{
		throw std::invalid_argument("User does not exist");
	}
	if (p == nullptr)
	{
		throw std::invalid_argument("User does not exist");
	}
	Admin * adm = dynamic_cast<Admin*>(p);
	Moderator * mod = dynamic_cast<Moderator*>(p);
	if (adm)
	{
		// I could have done this by just adding a setter for the .blocked prop of User but the
		// idea is that only Admins and moderators should be able to block other users.
		adm->block(user);
	}
	else if (mod)
	{
		mod->block(user);
	}
	else
	{
		throw std::invalid_argument("Only the admin and moderators are allowed to block users");
	}
}

void fmi::Fmibook::unblock(char * actor, char * who)
{
	User * p = getUserByNickname(actor);
	User * user = getUserByNickname(who);
	if (user == nullptr)
	{
		throw std::invalid_argument("User does not exist");
	}
	if (p == nullptr)
	{
		throw std::invalid_argument("User does not exist");
	}
	Admin * adm = dynamic_cast<Admin*>(p);
	Moderator * modr = dynamic_cast<Moderator*>(p);
	if (adm)
	{
		adm->unblock(user);
	}
	else if (modr)
	{
		modr->unblock(user);
	}
	else
	{
		throw std::invalid_argument("Only the admin and moderators are allowed to unblock users");
	}
}

void fmi::Fmibook::postImage(char * actor, char * content)
{
	User * u = this->getUserByNickname(actor);
	if (u == nullptr)
	{
		throw std::invalid_argument("User does not exist");
	}

	ImgPost * post = new ImgPost(content, nextId);
	++nextId;
	u->addPost(post);
}

void fmi::Fmibook::postLink(char * actor, char * content)
{
	User * u = this->getUserByNickname(actor);
	if (u == nullptr)
	{
		throw std::invalid_argument("User does not exist");
	}

	LinkPost * post = new LinkPost(content, nextId);
	++nextId;
	u->addPost(post);
}

void fmi::Fmibook::postText(char * actor, char * content)
{
	User * u = this->getUserByNickname(actor);
	if (u == nullptr)
	{
		throw std::invalid_argument("User does not exist");
	}

	TextPost * post = new TextPost(content, nextId);
	++nextId;
	u->addPost(post);
}

void fmi::Fmibook::removePost(char * actor, unsigned int id)
{
	User * p = this->getUserByNickname(actor);
	if (p == nullptr)
	{
		throw std::invalid_argument("User does not exist");
	}
	//p->removePost(id);
}

User * fmi::Fmibook::getUserByNickname(char * nickname)
{
	unsigned int count = this->users.count();
	User * p;
	for (int i = 0; i < count; i++)
	{
		p = this->users.getAt(i);
		if (strcmp(p->getNickname(), nickname) == 0)
		{
			return p;
		}
	}
	return nullptr;
}
