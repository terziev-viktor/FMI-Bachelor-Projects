#pragma once
#include "Word.h"
class Date : public Word
{
public:
	Date();
	Date(const String & value);
	int compare(const Word & other) override;
};

