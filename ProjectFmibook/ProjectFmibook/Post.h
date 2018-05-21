#pragma once
namespace fmi
{
	namespace posts
	{
		class Post
		{
		public:
			Post(char * content, unsigned int id);
			virtual ~Post();
			static const unsigned int MAX_CONTENT = 500;
			const char * getContent() const;
			virtual const char * asHTML() const = 0;

			const unsigned int getId() const;
		protected:
			void setContent(const char * content);
			
		private:
			unsigned int id;
			unsigned int ownerId;
			
			char content[MAX_CONTENT];
		};
	}
}


