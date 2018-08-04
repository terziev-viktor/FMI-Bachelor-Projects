#pragma once
#include "Vector.hpp"
#include "Word.h"
#include "WordsFactory.h"
using namespace WordsFactory;

class Message
{
public:
	Message();
	Message(const String & value, const Basic_WordFactory * factory);
	Message(const Message & other);
	~Message();
	void set(const String & value, const Basic_WordFactory * factory);
	float compare(const Word & word) const;
	const Vector<Word*> & get_words() const;
private:
	Vector<Word*> words;

	void set_message(const String & value, const Basic_WordFactory * factory);
};

std::ostream & operator<<(std::ostream & os, const Message & obj);

