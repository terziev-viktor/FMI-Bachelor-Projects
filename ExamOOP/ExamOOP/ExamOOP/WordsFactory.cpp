#include "WordsFactory.h"

Word * WordsFactory::Basic_WordFactory::create_word(const String & type, const String & value)
{
	if (type == "Simple")
	{
		return new Simple(value);
	}
	if (type == "Hashtag")
	{
		return new Hashtag(value);
	}
	if (type == "Date")
	{
		return new Date(value);
	}
	return nullptr;
}
