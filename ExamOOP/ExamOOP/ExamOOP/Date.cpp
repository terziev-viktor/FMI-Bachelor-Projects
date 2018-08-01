#include "Date.h"



Date::Date()
	:Word("", "Date")
{
}

Date::Date(const String & value)
	:Word(value, "Date")
{
	if (value.get_length() != 10)
	{
		throw std::exception("Not a date");
	}
	if (value[2] != '-' || value[5] != '-')
	{
		throw std::exception("Not a date");
	}
	for (size_t i = 0; i < value.get_length(); i++)
	{
		if (!((value[i] >= 0 && value[i] <= '9') || value[i] == '-'))
		{
			throw std::exception("Not a date");
		}
	}
}

Word * Date::get_copy() const
{
	return new Date(*this);
}

int Date::compare(const Word & other) const
{
	return 0;
}
