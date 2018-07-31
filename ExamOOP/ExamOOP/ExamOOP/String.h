#pragma once
#include "Vector.hpp"

class String
{
public:
	String();
	// Init a String that can hold capacity characters
	String(unsigned int capacity);
	// value should be a string ending with '\0'
	String(const char * value);
	String(const String & other);
	~String();
	unsigned int get_length() const;
	unsigned int get_capacity() const;
	void set_capacity(unsigned int length);
	const char * get_as_char_array() const;
	String substring(unsigned int from, unsigned int to) const;
	int find_first_of(const String & characters, unsigned int start_from = 0) const;
	bool contains(const char ch) const;
	Vector<String> split(const String & delims) const;
	bool ends_with(const String &) const;
	bool starts_with(const char ch) const;

	bool to_int(int &) const;
	static String to_String(int num);

	String & operator+=(const String & other);
	String & operator+=(const char ch);
	String & operator=(const String & other);
	bool operator==(const String & other) const;
	bool operator!=(const String & other) const;
	char operator[](unsigned int index) const;
	char & operator[](unsigned int index);
private:
	char * value;
	unsigned int length;
	unsigned int capacity;
};

String operator+(const String & a, const String & b);
std::ostream & operator<<(std::ostream&, const String &);
std::istream & operator>>(std::istream&, String & obj);
