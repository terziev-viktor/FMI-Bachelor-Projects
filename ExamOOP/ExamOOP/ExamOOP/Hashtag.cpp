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

float Hashtag::compare(const Word & other) const
{
	if (this->get_type() != other.get_type())
	{
		return 0.0;
	}
	String str1 = this->get_value().get_to_lower_case();
	String str2 = other.get_value().get_to_lower_case();
	if (str1 == str2)
	{
		return 1.0;
	}
	return 0.0;
}

