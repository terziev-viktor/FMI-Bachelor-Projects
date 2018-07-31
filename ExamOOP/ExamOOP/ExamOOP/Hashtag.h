#pragma once
#include "Word.h"
class Hashtag : public Word
{
public:
	Hashtag();
	Hashtag(const String & value);

	int compare(const Word & other) override;
};

