#pragma once
#include "Post.h"

namespace fmi
{
	namespace posts
	{
		class LinkPost : public Post
		{
		public:
			LinkPost(char * content, unsigned int id);
			~LinkPost();
			const char * asHTML() const;
		};


	}
}
