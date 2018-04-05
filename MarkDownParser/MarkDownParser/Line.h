#pragma once
class Line
{
public:
	Line();
	Line(char*);
	Line(const Line & l);
	~Line();

	bool getIsLoaded() const;
	void load(const char*);
	bool setChar(int at, char ch);
	void addChar(char ch);
	char getChar(int at) const;
	// expand with 'n' characters
	void expand(int n);
	void print();
private:
	char * buffer;
	int size;
	int index;
	const int INIT_SIZE = 1024;
	void cpy(char*);
	bool isLoaded;
};

