#pragma once
#include "Post.h"

namespace fmi
{
	namespace posts
	{
		class ImgPost : public Post
		{
		public:
			ImgPost(char * content, unsigned int id);
			~ImgPost();
			const char * asHTML() const;
		};

	}
}


