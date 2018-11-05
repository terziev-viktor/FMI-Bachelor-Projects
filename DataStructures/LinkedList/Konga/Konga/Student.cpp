#include "Student.h"


Student::Student()
{
	this->uni = UNI::UNKNOWN;
	this->name[0] = '\0';
}

Student::Student(const string & name, UNI uni)
{
	this->name = name;
	this->uni = uni;
}

const string & Student::get_name() const
{
	return name;
}

UNI Student::get_UNI() const
{
	return uni;
}

string UNI_to_string(UNI uni)
{
	switch (uni)
	{
	case FMI:
		return "FMI";
	case TU:
		return "TU";
	case UNWE:
		return "UNWE";
	default:
		return "UNKNOWN";
	}
}

UNI string_to_UNI(const string & str)
{
	if (str == "FMI")
	{
		return UNI::FMI;
	}
	if (str == "TU")
	{
		return UNI::TU;
	}
	if (str == "UNWE" || str == "UNSS")
	{
		return UNI::UNWE;
	}
	return UNI::UNKNOWN;
}

std::ostream & operator<<(std::ostream & out, const Student & obj)
{
	out << '(' << obj.get_name() << ", " << UNI_to_string(obj.get_UNI()) << ')';
	return out;
}
