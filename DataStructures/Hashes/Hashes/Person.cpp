#include "Person.h"



Person::Person()
	:left(nullptr), right(nullptr), grabbed_left(false), grabbed_right(false)
{

}

Person::Person(const string & n)
	: left(nullptr), right(nullptr), grabbed_left(false), grabbed_right(false)
{
	this->SetNickname(n);
}

Person & Person::SetLeft(Person * l)
{
	if (l == this || l == nullptr)
	{
		throw "No logic exception";
	}
	left = l;
	return *this;
}

Person & Person::SetRight(Person * r)
{
	if (r == this || r == nullptr)
	{
		throw "No logic exception";
	}
	right = r;
	return *this;
}

Person & Person::GrabLeft()
{
	grabbed_left = true;
	return *this;
}

Person & Person::GrabRight()
{
	grabbed_right = true;
	return *this;
}

std::ostream & operator<<(std::ostream & out, const Person & obj)
{
	if (obj.Left() && obj.GrabbedLeft())
	{
		out << obj.Left()->GetNickname() << " <---- ";
	}
	else
	{
		if (obj.Left())
		{
			out << obj.Left()->GetNickname() << " ----- ";
		}
	}
	out << obj.GetNickname();
	if (obj.Right() && obj.GrabbedRight())
	{
		out << " ----> " << obj.Right()->GetNickname();
	}
	else
	{
		if (obj.Right())
		{
			out << " ----- " << obj.Right()->GetNickname();
		}
	}
	return out;
}
