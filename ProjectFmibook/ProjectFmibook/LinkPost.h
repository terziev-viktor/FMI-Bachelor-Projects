#pragma once
#include "Post.h"

namespace fmi
{
	namespace posts
	{
		class LinkPost : public Post
		{
		public:
			LinkPost(const char * url, const char * description, unsigned int id, unsigned int ownerId);
			~LinkPost();
			static const unsigned int MAX_DESCR_LEN = 100;
			const char * asHTML() const;
			const char * getDescription() const;
		private:
			static const unsigned int MAX_HTML_SIZE = MAX_CONTENT + MAX_DESCR_LEN + 16;  // max content + space for html tag <a href='..'></a> + \0
			char description[MAX_DESCR_LEN];
		};


	}
}
