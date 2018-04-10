#pragma once
class Line
{
public:
	Line();
	Line(const char*);
	Line(const Line & l);
	~Line();

	bool getIsLoaded() const;
	void load(const char*);
	bool setChar(int at, char ch);
	char getChar(int at) const;
	// size of buffer
	int getSize() const;
	// number of symbols on the line
	int getLength() const;
	void setNewContent(char * newbuffer, int size);
	int getNextWordIndex();
	void resetWordIndex();
	void print();
private:
	char * buffer;
	int size;
	bool isLoaded;
	int wordindex;
};

