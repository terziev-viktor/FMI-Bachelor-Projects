#pragma once
#include <cstring>
#include <iostream>
enum UNI
{
	FMI, TU, UNWE, UNKNOWN
};

class Student
{
private:
	UNI uni;
	char name[100];
public:
	Student();

	Student(const char * name, UNI uni);
	
	const char * get_name() const;

	UNI get_UNI() const;
};

const char * UNIAsStr(UNI uni);

std::ostream & operator<<(std::ostream & out, const Student & obj);