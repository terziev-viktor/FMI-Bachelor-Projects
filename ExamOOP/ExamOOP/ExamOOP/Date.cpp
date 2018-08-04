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

float Date::compare(const Word & other) const
{
	if (this->get_type() != other.get_type())
	{
		return 0.0;
	}
	// dd-mm-dddd
	int days1, months1, years1, days2, months2, years2;
	bool success = false;
	success = this->get_value().substring(0, 2).to_int(days1);
	success = this->get_value().substring(3, 5).to_int(months1);
	success = this->get_value().substring(6, 10).to_int(years1);
	success = other.get_value().substring(0, 2).to_int(days2);
	success = other.get_value().substring(3, 5).to_int(months2);
	success = other.get_value().substring(6, 10).to_int(years2);
	if (!success)
	{
		throw std::exception("Invalid format of date word");
	}
	if (years1 != years2)
	{
		return 0.0;
	}
	int d = fabs(months1 - months2);
	d *= 30;
	if (d >= 100.0)
	{
		return 0.f;
	}
	d += fabs(days1 - days2);
	if (d >= 100.0)
	{
		return 0.0;
	}
	return 100.0 - d;
}
