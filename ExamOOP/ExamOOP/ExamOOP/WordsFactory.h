#pragma once
#include "Date.h"
#include "Hashtag.h"
#include "Simple.h"

namespace WordsFactory
{
	class Basic_WordFactory
	{
	public:
		virtual ~Basic_WordFactory() = default;

		virtual Word * create_word(const String & type, const String & value);
	};
};

