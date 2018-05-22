#pragma once
namespace fmi
{
	namespace posts
	{
		class Post
		{
		public:
			Post(const char * content, unsigned int id, unsigned int ownerId);
			virtual ~Post();

			static const unsigned int MAX_CONTENT = 500;
			
			virtual const char * asHTML() const = 0;

			const char * getContent() const;
			const unsigned int getId() const;
			const unsigned int getOwnerId() const;
		protected:
			void setContent(const char * content);
			
		private:
			unsigned int id;
			unsigned int ownerId;
			
			char content[MAX_CONTENT];
		};
	}
}


