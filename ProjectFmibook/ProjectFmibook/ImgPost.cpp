#include "ImgPost.h"
#include <iostream>

using fmi::posts::ImgPost;

fmi::posts::ImgPost::ImgPost(const char * content, unsigned int id, unsigned int ownerId)
	:Post(content, id, ownerId)
{
}

ImgPost::~ImgPost()
{
}

const char * ImgPost::asHTML() const
{
	// result should look something like this:
	// <img src='content'>
	int size = strlen(this->getContent()) + 13;
	char * result = new char[size]; // max content + space for html tag <img .. > + \0
	result[0] = '\0';
	strcat_s(result, size, "<img src='");
	strcat_s(result, size, this->getContent());
	strcat_s(result, size, "'>");

	return result;
}
