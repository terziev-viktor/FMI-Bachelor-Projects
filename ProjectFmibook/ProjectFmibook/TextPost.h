#pragma once
#include "Post.h"
namespace fmi
{
	namespace posts
	{
		class TextPost : public Post
		{
		public:
			TextPost(char * content, unsigned int id);
			~TextPost();
			const char * asHTML() const;
		};

	}
}


