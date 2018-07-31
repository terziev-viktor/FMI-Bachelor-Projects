#pragma once
#include "Vector.hpp"
#include "Word.h"

class Message
{
public:
	Message();
	Message(const String & value);
	void set(const String & value);
	const Vector<Word> & get_words() const;

private:
	Vector<Word> words;

	void set_message(const String & value);
};

