#include "Post.h"
#include <stdexcept>

using fmi::posts::Post;


fmi::posts::Post::Post(const char * content, unsigned int id, unsigned int ownerId)
{
	this->setContent(content);
	this->id = id;
	this->ownerId = ownerId;
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

const unsigned int fmi::posts::Post::getOwnerId() const
{
	return this->ownerId;
}

void Post::setContent(const char * content)
{
	if (content == nullptr)
	{
		throw std::invalid_argument("Invalid content for post");
	}
	strcpy_s(this->content, sizeof(this->content), content);
}
