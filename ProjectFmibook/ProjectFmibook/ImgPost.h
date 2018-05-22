#pragma once
#include "Post.h"

namespace fmi
{
	namespace posts
	{
		class ImgPost : public Post
		{
		public:
			ImgPost(const char * content, unsigned int id, unsigned int ownerId);
			~ImgPost();
			const char * asHTML() const;
		private:
			static const unsigned int MAX_HTML_SIZE = MAX_CONTENT + 12;
		};

	}
}


