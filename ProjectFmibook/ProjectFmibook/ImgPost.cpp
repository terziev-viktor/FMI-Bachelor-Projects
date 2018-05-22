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
	char result[MAX_HTML_SIZE]; // max content + space for html tag <img .. >
	result[0] = '\0';
	strcat_s(result, MAX_HTML_SIZE, "<img src='");
	strcat_s(result, MAX_HTML_SIZE, this->getContent());
	strcat_s(result, MAX_HTML_SIZE, "'>");

	return result;
}
