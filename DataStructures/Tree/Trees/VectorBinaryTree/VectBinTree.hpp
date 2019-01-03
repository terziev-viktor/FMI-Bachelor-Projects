#pragma once
#include <stdexcept>
#include <vector>
using std::vector;

template<typename T>
struct TValue
{
	bool free;
	T value;
	TValue() :free(true) {}
};

template<typename T>
class DataVect
{
public:
	TValue<T> * data;

	size_t size;

	size_t current;

	void Expand(size_t to)
	{
		if (data == nullptr)
		{
			this->data = new TValue<T>[to + 1];
			this->size = to + 1;
			this->current = 0;
		}
		else
		{
			TValue<T> * values = new TValue<T>[to + 1];
			for (size_t i = 0; i < this->size; ++i)
			{
				values[i] = this->data[i];
			}
			this->size = to + 1;
			delete[] this->data;
			this->data = values;
		}
	}

	DataVect()
		:data(nullptr),size(0),current(0)
	{

	}

	~DataVect()
	{
		if (this->data)
		{
			delete[] this->data;
		}
	}

	size_t Size() const
	{
		return this->size;
	}

	TValue & operator[](size_t i)
	{
		if (i >= this->size)
		{
			this->expand(i);
		}
		return this->data[i];
	}
};

// T is the type of data and Comparator is a callable object that returns value < 0 if a < b, value > 0 if a > b and value == 0 if a == b
template<typename T, typename Comparator>
class VectBinTree
{
private:
	DataVect<T> data;
	Comparator compare;
	size_t current;
	TValue * Find() const
	{
		size_t c = 0;
		while (this->data[c].value != value)
		{
			if (this->data[c].free)
			{
				return nullptr;
			}
			if (compare(value, this->data[c].value)
			{
				c = 2 * c + 1;
			}
			else
			{
				c = 2 * c + 2;
			}
			if (c >= this->data.Size())
			{
				return nullptr;
			}
		}
		return &this->data[c];
	}
public:
	VectBinTree();

	bool Empty() const;

	const T & Root() const
	{
		if (this->size)
		{
			return this->data[0].value;
		}
		throw std::exception("The tree is empty");
	}

	bool Contains(const T & value) const
	{
		return this->Find(value) != nullptr;
	}

	void Insert(const T & value)
	{
		
	}
};

template<typename T, typename Comparator>
inline VectBinTree<T, Comparator>::VectBinTree()
{
	this->current = 0;
}

template<typename T, typename Comparator>
inline bool VectBinTree<T, Comparator>::Empty() const
{
	return this->data.Size();
}
