#include "TextPost.h"
#include <string>
using fmi::posts::TextPost;


fmi::posts::TextPost::TextPost(const char * content, unsigned int id, unsigned int ownerId)
	:Post(content, id, ownerId)
{
}

TextPost::~TextPost()
{
}

const char * TextPost::asHTML() const
{
	int size = strlen(this->getContent()) + 8;
	char * result = new char[size]; // max content + space for html tag <p>...</p> + \0
	result[0] = '\0';
	strcat_s(result, size, "<p>");
	strcat_s(result, size, this->getContent());
	strcat_s(result, size, "</p>");

	return result;
}
