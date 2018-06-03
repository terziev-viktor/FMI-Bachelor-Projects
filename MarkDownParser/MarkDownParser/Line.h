#pragma once
class Line
{
public:
	Line();
	Line(const char*);
	~Line();

	const bool getIsLoaded() const;
	void load(const char*);
	const char getChar(int at) const;
	// size of buffer
	const int getSize() const;
	// number of symbols on the line
	const int getLength() const;
	bool setNewContent(char * newbuffer, int size);
	int getNextWordIndex();
	void resetWordIndex();
	const bool isHeading() const;
	// Prints the buffer on the std output. This method is for debug purpuses.
	void print() const;
private:
	char * buffer;
	int size;
	bool isLoaded;
	int wordindex;
};

