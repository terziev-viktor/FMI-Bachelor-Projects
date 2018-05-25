#pragma once

namespace fmi
{
	namespace tools
	{
		// Interface for a dynamically allocated array of elements
		template<class T>
		class DynamicArray
		{
		public:
			virtual void add(T * item) = 0;
			virtual T * getAt(unsigned int index) const = 0;
			virtual void removeAt(unsigned int index) = 0;
			virtual bool isEmplty() = 0;
			virtual void clear() = 0;
			virtual unsigned int count() = 0;
		};

	}
}

