#include "Hashtag.h"



Hashtag::Hashtag()
	:Word("", "Hashtag")
{
}

Hashtag::Hashtag(const String & value)
	:Word(value, "Hashtag")
{
}

int Hashtag::compare(const Word & other)
{
	return 0;
}

