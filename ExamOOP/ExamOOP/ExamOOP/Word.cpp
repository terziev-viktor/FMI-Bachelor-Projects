#include "Word.h"

Word::Word()
{
	this->value = "";
	this->type = "DEFAULT";
}

Word::Word(const String & value, const String & type)
{
	this->set_value(value);
	this->type = type;
}

const String & Word::get_value() const
{
	return this->value;
}

void Word::set_value(const String & new_value)
{
	this->value = new_value;
}

String Word::get_type() const
{
	return this->type;
}

float Word::compare(const Word & other) const
{
	int len_min = this->get_value().get_length() <= other.get_value().get_length() ? this->get_value().get_length() : other.get_value().get_length();
	int len_max = this->get_value().get_length() >= other.get_value().get_length() ? this->get_value().get_length() : other.get_value().get_length();
	int same_symbol_count = 0;
	String str1 = this->get_value().get_to_lower_case();
	String str2 = other.get_value().get_to_lower_case();
	for (int i = 0; i < len_min; i++)
	{
		if (str1.contains(str2[i]))
		{
			++same_symbol_count;
		}
	}
	float persent = ((float)same_symbol_count / len_max) * 100.0;
	return persent;
}

Word * Word::get_copy() const
{
	return new Word(*this);
}

std::ostream & Word::operator<<(std::ostream & os) const
{
	os << this->get_value();
	return os;
}
