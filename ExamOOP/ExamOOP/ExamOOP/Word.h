#pragma once
#include "String.h"

class Word
{
public:
	Word();
	Word(const String & value, const String & type);
	const String & get_value() const;
	void set_value(const String & new_value);
	String get_type() const;
	virtual int compare(const Word & other) const;
	virtual Word * get_copy() const;
	std::ostream & operator<<(std::ostream & os) const;
private:
	String value;
	String type;
};

