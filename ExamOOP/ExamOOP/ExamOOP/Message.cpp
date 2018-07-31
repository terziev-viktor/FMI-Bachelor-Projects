#include "Message.h"

Message::Message()
{
}

Message::Message(const String & value)
{
	this->set_message(value);
}

void Message::set(const String & value)
{
	this->set_message(value);
}

const Vector<Word>& Message::get_words() const
{
	return this->words;
}

void Message::set_message(const String & value)
{
	unsigned int len = value.get_length();
	for (size_t i = 0; i < len; i++)
	{
		// todo
	}
}
