#include "LinkPost.h"
#include <iostream>
using fmi::posts::LinkPost;


fmi::posts::LinkPost::LinkPost(const char * url, const char * description, unsigned int id, unsigned int ownerId)
	:Post(url, id, ownerId)
{
	strcpy_s(this->description, sizeof(this->description), description);
}

LinkPost::~LinkPost()
{
}

const char * fmi::posts::LinkPost::asHTML() const
{
	unsigned int size = strlen(this->getContent()) + strlen(this->getDescription()) + 16;
	char * result = new char[size];
	result[0] = '\0';
	strcat_s(result, size, "<a href='");
	strcat_s(result, size, this->getContent());
	strcat_s(result, size, "'>");
	strcat_s(result, size, this->description);
	strcat_s(result, size, "</a>");

	return result;
}

const char * fmi::posts::LinkPost::getDescription() const
{
	return this->description;
}
