#include "Post.h"
#include <stdexcept>

using fmi::posts::Post;

Post::Post(char * content, unsigned int id)
{
	this->setContent(content);
	this->id = id;
}

Post::~Post()
{
}

const char * Post::getContent() const
{
	return this->content;
}

const unsigned int Post::getId() const
{
	return this->id;
}

void Post::setContent(const char * content)
{
	if (content == nullptr)
	{
		throw std::invalid_argument("Invalid content for post");
	}
	strcpy_s(this->content, sizeof(this->content), content);
}
