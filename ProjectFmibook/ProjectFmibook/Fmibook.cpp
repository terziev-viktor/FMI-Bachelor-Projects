#include "Fmibook.h"
#include "Moderator.h"
#include "Post.h"

#include <stdexcept>
#include <iostream>
#include <fstream>
using std::cout;
using std::endl;
using fmi::Fmibook;
using fmi::users::User;
using fmi::posts::Post;

fmi::Fmibook::Fmibook(const char * adminNickname, unsigned short adminAge)
{
	this->nextId = 1;
	Admin * theAdmin = Admin::getInstancePointer(adminNickname, adminAge, this->nextId);
	this->users.add(theAdmin);
	++this->nextId;
}

Fmibook::~Fmibook()
{
}

void fmi::Fmibook::addModerator(const char * actor, const char * nickname, unsigned short age)
{
	User * p = this->getUserByNickname(actor);
	if (p == nullptr)
	{
		throw std::invalid_argument("User does not exist");
	}
	Admin * theAdmin = dynamic_cast<Admin*>(p);
	if (theAdmin)
	{
		theAdmin->addUser(new Moderator(nickname, age, nextId), this->users);
		++nextId;
	}
	else
	{
		throw std::invalid_argument("Only the admin can add a new moderator");
	}
}

void fmi::Fmibook::addUser(const char * actor, const char * nickname, unsigned short age)
{
	User * p = this->getUserByNickname(actor);
	if (p == nullptr)
	{
		throw std::invalid_argument("User does not exist");
	}
	Admin * theAdmin = dynamic_cast<Admin*>(p);
	if (theAdmin)
	{
		theAdmin->addUser(new BasicUser(nickname, age, nextId), this->users);
		++nextId;
	}
	else
	{
		throw std::invalid_argument("Only the admin can add a new user");
	}
}

void fmi::Fmibook::removeUser(const char * actor, const char * nickname)
{
	User * adminPointer = this->getUserByNickname(actor);
	User * userToRemovePointer = this->getUserByNickname(nickname);
	if (userToRemovePointer == nullptr)
	{
		throw std::invalid_argument("User does not exist");
	}
	if (adminPointer == nullptr)
	{
		throw std::invalid_argument("User does not exist");
	}
	Admin * theAdmin = dynamic_cast<Admin*>(adminPointer);
	if (theAdmin)
	{
		theAdmin->removeUser(userToRemovePointer->getId(), this->users, this->posts);
	}
	else
	{
		throw std::invalid_argument("Only the admin can remove a user");
	}
}

void fmi::Fmibook::block(const char * actor, const char * nickname)
{
	User * p = getUserByNickname(actor);
	User * userToBlock = getUserByNickname(nickname);
	if (userToBlock == nullptr)
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
		adm->block(userToBlock);
	}
	else if (mod)
	{
		mod->block(userToBlock);
	}
	else
	{
		throw std::invalid_argument("Only the admin and moderators are allowed to block users");
	}
}

void fmi::Fmibook::unblock(const char * actor, const char * who)
{
	User * p = getUserByNickname(actor);
	User * userToUnblock = getUserByNickname(who);
	if (userToUnblock == nullptr)
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
		adm->unblock(userToUnblock);
	}
	else if (modr)
	{
		modr->unblock(userToUnblock);
	}
	else
	{
		throw std::invalid_argument("Only the admin and moderators are allowed to unblock users");
	}
}

void fmi::Fmibook::postImage(const char * actor, const char * content)
{
	User * u = this->getUserByNickname(actor);
	if (u == nullptr)
	{
		throw std::invalid_argument("User does not exist");
	}

	ImgPost * post = new ImgPost(content, nextId, u->getId());
	++nextId;
	u->addPost(post, this->posts);
}

void fmi::Fmibook::postLink(const char * actor, const char * url, const char * desctiption)
{
	User * u = this->getUserByNickname(actor);
	if (u == nullptr)
	{
		throw std::invalid_argument("User does not exist");
	}

	LinkPost * post = new LinkPost(url, desctiption, nextId, u->getId());
	++nextId;
	u->addPost(post, this->posts);
}

void fmi::Fmibook::postText(const char * actor, const char * content)
{
	User * u = this->getUserByNickname(actor);
	if (u == nullptr)
	{
		throw std::invalid_argument("User does not exist");
	}

	TextPost * post = new TextPost(content, nextId, u->getId());
	++nextId;
	u->addPost(post, this->posts);
}

void fmi::Fmibook::removePost(const char * actor, unsigned int id)
{
	User * p = this->getUserByNickname(actor);
	if (p == nullptr)
	{
		throw std::invalid_argument("User does not exist");
	}
	p->removePost(id, this->posts);
}

void fmi::Fmibook::viewPost(const char * actor, unsigned int id)
{
	Post * p;
	for (unsigned int i = 0; i < this->posts.count(); i++)
	{
		p = posts.getAt(i);
		if (p->getId() == id)
		{
			const char * content = p->asHTML();

			char idstr[33];
			_itoa_s(id, idstr, 10);
			int outFileNameSize = strlen(actor) + strlen(idstr) + 14;
			char * outFileName = new char[outFileNameSize]; // nickname + viewpost + id + .html + \0
			outFileName[0] = '\0';
			strcat_s(outFileName, outFileNameSize, actor);
			strcat_s(outFileName, outFileNameSize, "viewPost");
			strcat_s(outFileName, outFileNameSize, idstr);
			strcat_s(outFileName, outFileNameSize, ".html");

			std::ofstream out;
			out.open(outFileName);
			if (!out)
			{
				delete[] outFileName;
				throw "Could not create an output html file";
			}
			out << content << endl;
			cout << content << endl;
			delete[] content;
			out.close();
			delete[] outFileName;
			return;
		}
	}
	throw "This post does not exist";
}

void fmi::Fmibook::viewAllPosts(const char * actorNickname, const char * ofwhoNickname)
{
	User * actor = this->getUserByNickname(actorNickname);
	User * ofWho = this->getUserByNickname(ofwhoNickname);
	if (actor == nullptr || ofWho == nullptr)
	{
		throw "User does not exists";
	}

	unsigned int count = this->posts.count();
	cout << actor->getNickname() << " views " << ofWho->getNickname() << "'s posts:" << endl;
	std::ofstream out;
	unsigned int filenameSize = strlen(actor->getNickname()) + strlen(ofWho->getNickname()) + 21;
	char * fileName = new char[filenameSize];
	// building file name string
	fileName[0] = '\0';
	strcat_s(fileName, filenameSize, actor->getNickname());
	strcat_s(fileName, filenameSize, "viewsAllPostsOf");
	strcat_s(fileName, filenameSize, ofWho->getNickname());
	strcat_s(fileName, filenameSize, ".html");
	// -------------------------
	out.open(fileName);
	if (!out)
	{
		delete[] fileName;
		out.close();
		throw "Could not create output html file";
	}
	for (unsigned int i = 0; i < count; i++)
	{
		Post * p = this->posts.getAt(i);
		if (p->getOwnerId() == ofWho->getId())
		{
			const char * html = p->asHTML();
			cout << html << endl;
			out << html << endl;
			delete[] html;
		}
	}
	out.close();
	delete[] fileName;
}

void fmi::Fmibook::changeNickname(const char * actor, const char * newNickname)
{
	User * u = getUserByNickname(actor);
	u->changeNickname(newNickname, users);
}

void fmi::Fmibook::info()
{
	// When Admin is created the counter increases so we substract 1
	cout << "Number of Moderators: " << Moderator::getObjectsCount() - 1 << endl;
	// Basic users are all users that are not moderators
	cout << "Number of Basic Users: " << this->users.count() - Moderator::getObjectsCount() << endl;
	unsigned int countOfBlockedUsers = 0;
	cout << "Blocked Users:" << endl;
	for (unsigned int i = 0; i < this->users.count(); i++)
	{
		User * u = this->users.getAt(i);
		if (u->isBlocked())
		{
			countOfBlockedUsers++;
			cout << u->getNickname() << endl;
		}
	}
	cout << "(count " << countOfBlockedUsers << ')' << endl;
}

User * fmi::Fmibook::getUserByNickname(const char * nickname)
{
	unsigned int count = this->users.count();
	User * p;
	for (unsigned int i = 0; i < count; i++)
	{
		p = this->users.getAt(i);
		if (strcmp(p->getNickname(), nickname) == 0)
		{
			return p;
		}
	}
	return nullptr;
}
