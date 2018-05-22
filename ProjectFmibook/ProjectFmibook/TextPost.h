#pragma once
#include "Post.h"
namespace fmi
{
	namespace posts
	{
		class TextPost : public Post
		{
		public:
			TextPost(const char * content, unsigned int id, unsigned int ownerId);
			~TextPost();
			const char * asHTML() const;
		private:
			static const unsigned int MAX_HTML_SIZE = MAX_CONTENT + 7;
		};

	}
}


