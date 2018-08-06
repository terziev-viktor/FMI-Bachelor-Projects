#include "WordsFactory.h"

Word * WordsFactory::Basic_WordFactory::create_word(const String & value) const
{
	if (Date::check_format(value))
	{
		return new Date(value);
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
