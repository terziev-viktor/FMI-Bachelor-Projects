#pragma once
#include "WordsFactory.h"
#include "Message.h"

using namespace WordsFactory;

class Document
{
public:
	Document(const Basic_WordFactory * concrete_factory);
	void load(const String & path);
	const Vector<Message> & get_messages() const;
	Vector<Hashtag> get_hashtags() const;
	const Basic_WordFactory * get_factory() const;
	Document & operator+(const Message & message); // adds the message
	Document & operator+=(const Document & message); // adds other's messages
	const Message & operator[](const String & word) const;
	Vector<Message> filter_messages(const String & filter) const;
private:
	struct Cmp_Index
	{
		float comparison;
		size_t index;
	};
	const Basic_WordFactory * factory;
	Vector<Message> messages;
};

std::ostream & operator<<(std::ostream & os, const Document & obj);
