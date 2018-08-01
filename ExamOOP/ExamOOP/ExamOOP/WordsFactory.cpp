#include "WordsFactory.h"

Word * WordsFactory::Basic_WordFactory::create_word(const String & value) const
{
	Word * w = nullptr;
	try
	{
		Word * w = new Date(value);
		return w;
	}
	catch (const std::exception&)
	{
		if (w)
		{
			delete w;
		}
	}
	return new Word(value, "Simple");
}

Word * WordsFactory::Concrete_WordFactory::create_word(const String & value) const
{
	if (value[0] == '#')
	{
		return new Hashtag(value);
	}
	return Basic_WordFactory::create_word(value);
}
