#pragma once
#include <string>
#include <ostream>
using std::string;
class Person
{
private:
	Person * left;
	Person * right;
	bool grabbed_left;
	bool grabbed_right;
	std::string nickname;
public:
	Person();
	Person(const string &);

	const string & GetNickname() const
	{
		return nickname;
	}

	void SetNickname(const string & nickname)
	{
		if (nickname.empty())
		{
			throw "No nickname is not a nickname u dumb fck";
		}
		this->nickname = nickname;
	}

	bool IsNeibhour(const Person & p) const
	{
		return this->left->GetNickname() == p.GetNickname() || this->right->GetNickname() == p.GetNickname();
	}

	Person & SetLeft(Person * l);

	Person & SetRight(Person * r);

	Person & GrabLeft();

	Person & GrabRight();

	bool GrabbedRight() const { return grabbed_right; }

	bool GrabbedLeft() const { return grabbed_left; }

	bool IsLeftNeibhourOf(const Person & p)
	{
		return p.left == this;
	}

	bool IsRightNeibhourOf(const Person & p)
	{
		return p.right == this;
	}

	void SwapNeibhoursWith(Person & B)
	{
		Person * A = this;
		if (this->IsLeftNeibhourOf(B))
		{
			Person * ALeft = A->left;
			Person * BRight = B.right;
			A->right = B.right;
			A->left = &B;
			B.right->left = A;
			B.right = A;
			B.left = ALeft;
			ALeft->right = &B;
		}
		else if (this->IsRightNeibhourOf(B))
		{
			Person * ARight = A->right;
			Person * BLeft = B.left;
			A->right = &B;
			A->left = B.left;
			B.left = A;
			B.right = ARight;
			ARight->left = &B;
			BLeft->right = A;
		}
	}

	bool IsFree() const
	{
		return !this->GrabbedLeft()
			&& !this->GrabbedRight()
			&& !this->Left()->GrabbedRight()
			&& !this->Right()->GrabbedLeft();
	}

	Person & ReleaseLeft()
	{
		this->grabbed_left = false;
		return *this;
	}

	Person & ReleaseRight() 
	{
		this->grabbed_right = false; 
		return *this;
	}

	const Person * Left() const
	{
		return left;
	}

	const Person * Right() const
	{
		return right;
	}
};

std::ostream & operator<<(std::ostream & out, const Person & obj);

