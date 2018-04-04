#pragma once
class Line
{
public:
	Line();
	~Line();

	void expand();
private:
	char * buffer;
	int size;
	int index;
};

