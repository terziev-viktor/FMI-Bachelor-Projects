#pragma once
#include "Word.h"
class Date : public Word
{
public:
	Date();
	Date(const String & value);
	float compare(const Word & other) const override;
	Word * get_copy() const override;
};

