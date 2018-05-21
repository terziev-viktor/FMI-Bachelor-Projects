#include "ImgPost.h"
#include <string>

using fmi::posts::ImgPost;


ImgPost::ImgPost(char * content, unsigned int id)
	:Post(content, id)
{
}

ImgPost::~ImgPost()
{
}

const char * ImgPost::asHTML() const
{
	// result should look something like this:
	// <img src='content'>
	char result[MAX_CONTENT + 12]; // max content + space for html tag <img .. >
	result[0] = '\0';
	strcat_s(result, "<img src='");
	strcat_s(result, this->getContent());
	strcat_s(result, "'>");

	return result;
}
