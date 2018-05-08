#pragma once
class IUser
{
public:
	IUser();
	~IUser();
	char* getNickname();
	unsigned short getAge();
	void setNickname(char *);
	void setAge(unsigned short age);
protected:
	bool isBlocked() { return this->isblocked; }
private:
	char nickname[100];
	unsigned short age;
	bool isblocked;
};

