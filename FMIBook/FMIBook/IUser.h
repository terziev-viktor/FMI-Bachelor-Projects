#pragma once
class IUser
{
public:
	IUser();
	~IUser();
	char* getNickname();
	unsigned short getAge();
protected:
	bool isBlocked() { return this->isBlocked; }
private:
	char nickname[100];
	unsigned short age;
	bool isBlocked;
};

