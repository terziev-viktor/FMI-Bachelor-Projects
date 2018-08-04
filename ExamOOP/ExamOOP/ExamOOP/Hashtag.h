#pragma once
#include "Word.h"
class Hashtag : public Word
{
public:
	Hashtag();
	Hashtag(const String & value);
	float compare(const Word & other) const override;
	Word * get_copy() const override;
};

