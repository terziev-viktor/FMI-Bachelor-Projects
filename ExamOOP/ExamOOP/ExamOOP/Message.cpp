#include "Message.h"

Message::Message()
{
}

Message::~Message()
{
	for (size_t i = 0; i < this->words.count(); i++)
	{
		Word * w = words[i];
		delete w;
	}
}
Message & Message::operator=(const Message & other)
{
	for (size_t i = 0; i < this->words.count(); i++)
	{
		delete words[i];
	}
	this->words.clear();
	const Vector<Word*> other_words = other.get_words();
	for (size_t i = 0; i < other_words.count(); i++)
	{
		this->words.add(other_words[i]->get_copy());
	}
	return *this;
}
Message::Message(const String & value, const Basic_WordFactory * factory)
{
	this->set_message(value, factory);
}

Message::Message(const Message & other)
{
	for (size_t i = 0; i < other.get_words().count(); i++)
	{
		this->words.add(other.get_words()[i]->get_copy());
	}
}

void Message::set(const String & value, const Basic_WordFactory * factory)
{
	this->set_message(value, factory);
}

float Message::compare(const Word & word) const
{
	float result = 0.0f;
	for (size_t i = 0; i < this->get_words().count(); i++)
	{
		result += word.compare(*this->get_words()[i]);
	}
	return result;
}

const Vector<Word*>& Message::get_words() const
{
	return this->words;
}

void Message::set_message(const String & value, const Basic_WordFactory * factory)
{
	for (size_t i = 0; i < this->words.count(); i++)
	{
		Word * w = words[i];
		delete w;
	}
	this->words.clear();
	Vector<String> str_words = value.extract("1234567890QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm-#");
	for (size_t i = 0; i < str_words.count(); i++)
	{
		Word * word = factory->create_word(str_words[i]);
		this->words.add(word);
	}
}

std::ostream & operator<<(std::ostream & os, const Message & obj)
{
	const Vector<Word*> & words = obj.get_words();
	for (size_t i = 0; i < words.count(); i++)
	{
		os << words[i]->get_value() << " ";
	}
	return os;
}
