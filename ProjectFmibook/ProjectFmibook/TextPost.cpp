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
	char result[MAX_HTML_SIZE]; // max content + space for html tag <p>...</p>
	result[0] = '\0';
	strcat_s(result, MAX_HTML_SIZE, "<p>");
	strcat_s(result, MAX_HTML_SIZE, this->getContent());
	strcat_s(result, MAX_HTML_SIZE, "</p>");

	return result;
}
