#pragma once
class Line
{
public:
	Line();
	Line(const char*);
	~Line();

	bool getIsLoaded() const;
	void load(const char*);
	char getChar(int at) const;
	// size of buffer
	int getSize() const;
	// number of symbols on the line
	int getLength() const;
	bool setNewContent(char * newbuffer, int size);
	int getNextWordIndex();
	void resetWordIndex();
	bool isHeading();
	// Prints the buffer on the std output. This method is for debug purpuses.
	void print();
private:
	char * buffer;
	int size;
	int length;
	bool isLoaded;
	int wordindex;
};

