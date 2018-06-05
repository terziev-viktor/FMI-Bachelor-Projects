#pragma once
template<class T>
class IIterator
{
public:
	virtual ~IIterator() = default;

	virtual const T & first() const = 0;
	virtual void next() = 0;
	virtual const T & current() const = 0;
	virtual bool hasNext() const = 0;
	virtual bool isDone() const = 0;
};

