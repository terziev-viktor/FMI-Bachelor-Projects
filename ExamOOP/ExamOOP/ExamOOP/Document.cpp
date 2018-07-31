#include "Document.h"


Document::Document(const Basic_WordFactory * concrete_factory)
{
	this->factory = concrete_factory;
}

void Document::load(const String & path)
{
	// todo: read 280 chars a line and make a message for each line in the txt file provided with <path>
}

const Vector<Message> & Document::get_messages() const
{
	return this->messages;
}

Vector<Word> Document::get_hashtags() const
{
	// todo
	return Vector<Word>();
}

Document & Document::operator+(Message & message)
{
	this->messages.add(message);
	return *this;
}

Document & Document::operator+=(const Document & other)
{
	this->messages.clear();
	this->messages += other.get_messages();
	return *this;
}

Message & Document::operator[](int i)
{
	return this->messages[i];
}

const Message & Document::operator[](int i) const
{
	return this->messages[i];
}

Vector<Word> Document::filter_words(const String & filter) const
{
	// todo
	return Vector<Word>();
}

Document::~Document()
{
	delete this->factory;
}
