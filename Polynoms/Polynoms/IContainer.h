#pragma once
#include "IIterator.h"

template<class T>
class IContainer
{
public:
	virtual ~IContainer() = default;
	virtual IIterator<T> * createIterator() = 0;
};