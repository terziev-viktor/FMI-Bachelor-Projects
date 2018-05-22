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
	char result[MAX_HTML_SIZE];
	result[0] = '\0';
	strcat_s(result, MAX_HTML_SIZE, "<a href='");
	strcat_s(result, MAX_HTML_SIZE, this->getContent());
	strcat_s(result, MAX_HTML_SIZE, "'>");
	strcat_s(result, MAX_HTML_SIZE, this->description);
	strcat_s(result, MAX_HTML_SIZE, "</a>");

	return result;
}

const char * fmi::posts::LinkPost::getDescription() const
{
	return this->description;
}
