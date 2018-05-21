#pragma once
#include "Moderator.h"

namespace fmi
{
	namespace users
	{
		// There should be only one admin so we are using the singleton design pattern
		class Admin : public Moderator
		{
		public:
			// return a pointer because Im storing pointers in my data collection (List<T>)
			static Admin * getInstancePointer(const char * nickname, unsigned short age, unsigned int id);
		private:
			Admin();
			Admin(const char * nickname, unsigned short age, unsigned int id);
			~Admin();
			Admin(const Admin&);
			void operator=(const Admin&);
		};
	}
}


