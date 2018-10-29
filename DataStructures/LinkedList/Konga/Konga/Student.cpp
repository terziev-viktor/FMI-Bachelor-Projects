#include "Student.h"


Student::Student()
{
	this->uni = UNI::UNKNOWN;
	this->name[0] = '\0';
}

Student::Student(const char * name, UNI uni)
{
	strcpy_s(this->name, sizeof(this->name), name);
	this->uni = uni;
}

const char * Student::get_name() const
{
	return name;
}

UNI Student::get_UNI() const
{
	return uni;
}

const char * UNIAsStr(UNI uni)
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

std::ostream & operator<<(std::ostream & out, const Student & obj)
{
	out << '(' << obj.getName() << ", " << UNIAsStr(obj.getUNI()) << ')';
	return out;
}
