#pragma once
#include "WordsFactory.h"
#include "Message.h"

using namespace WordsFactory;

class Document
{
public:
	Document(const Basic_WordFactory * concrete_factory);
	Document(const Document & other);
	~Document();
	void load(const String & path);
	const Vector<Message*> & get_messages() const;
	Vector<Hashtag> get_hashtags() const;
	
	Document & operator+(const Message & message); // adds the message
	Document & operator+=(const Document & message); // adds the message
	Message & operator[](int i);
	const Message & operator[](int i) const;
	Vector<Word> filter_words(const String & filter) const;
private:
	const Basic_WordFactory * factory;
	Vector<Message*> messages;
};

std::ostream & operator<<(std::ostream & os, const Document & obj);
