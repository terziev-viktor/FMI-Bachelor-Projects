#include "Hashtag.h"



Hashtag::Hashtag()
	:Word("", "Hashtag")
{
}

Hashtag::Hashtag(const String & value)
	:Word(value, "Hashtag")
{
}

Word * Hashtag::get_copy() const
{
	return new Hashtag(*this);
}

int Hashtag::compare(const Word & other) const
{
	return 0;
}

