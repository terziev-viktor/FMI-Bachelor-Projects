#pragma once
#include <iostream>

#include <string>
using std::string;

enum UNI
{
	FMI, TU, UNWE, UNKNOWN
};

class Student
{
public:
	UNI uni;
	string name;
public:
	Student();

	Student(const string & name, UNI uni);
	
	const string & get_name() const;

	UNI get_UNI() const;
};

string UNI_to_string(UNI uni);

UNI string_to_UNI(const string & str);

std::ostream & operator<<(std::ostream & out, const Student & obj);