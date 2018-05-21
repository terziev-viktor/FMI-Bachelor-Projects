#include "TextPost.h"

using fmi::posts::TextPost;

TextPost::TextPost(char * content, unsigned int id)
	:Post(content, id)
{
}

TextPost::~TextPost()
{
}

const char * TextPost::asHTML() const
{
	return "Text Post";
}
