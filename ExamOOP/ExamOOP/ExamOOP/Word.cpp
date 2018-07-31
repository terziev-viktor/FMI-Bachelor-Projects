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
	return this->type;
}

void Word::set_value(const String & new_value)
{
	this->value = new_value;
}

String Word::get_type() const
{
	return this->type;
}

int Word::compare(const Word & other)
{
	// todo: default comparrison
	return 0;
}
