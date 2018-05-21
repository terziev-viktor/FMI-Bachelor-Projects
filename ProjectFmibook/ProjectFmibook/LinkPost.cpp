#include "LinkPost.h"

using fmi::posts::LinkPost;

LinkPost::LinkPost(char * content, unsigned int id)
	:Post(content, id)
{

}


LinkPost::~LinkPost()
{
}

const char * fmi::posts::LinkPost::asHTML() const
{
	return "Link Post";
}
