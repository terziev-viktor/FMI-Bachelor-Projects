#pragma once

class cstring
{
public:
	cstring();
	// Init a cstring that can hold <capacity> characters
	cstring(unsigned int capacity);
	// value should be a string ending with '\0'
	cstring(const char * value);
	cstring(const cstring & other);
	~cstring();
	unsigned int get_length() const;
	unsigned int get_capacity() const;
	void set_capacity(unsigned int length);
	const char * get_as_char_array() const;
	bool contains(const char ch) const;

	bool ends_with(const cstring &) const;
	bool starts_with(const char ch) const;
	/*operators*/
	cstring & operator=(const cstring & other);
	bool operator==(const cstring & other) const;
	bool operator!=(const cstring & other) const;
	char operator[](unsigned int index) const;
	char & operator[](unsigned int index);
private:
	char * value;
	unsigned int length;
	unsigned int capacity;
};