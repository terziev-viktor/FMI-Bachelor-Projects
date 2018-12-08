#pragma once
#include <stdexcept>
#include <vector>
using std::vector;

template<typename T>
class DataVect
{
private:
	struct TValue
	{
		bool free;
		T value;
	};
	TValue * data;
	size_t size;
	size_t current;
	void expand(size_t to)
	{
		if (data == nullptr)
		{
			this->data = new TValue[to + 1];
			this->size = to + 1;
			for (size_t i = 0; i < this->size; ++i)
			{
				this->data[i].free = true;
			}
		}
		else
		{
			TValue * values = new TValue[to + 1];
			for (size_t i = 0; i < this->size; ++i)
			{
				values[i] = this->data[i];
			}
			for (size_t i = this->size; i < to + 1; ++i)
			{
				values[i].free = true;
			}
			this->size = to + 1;
			delete[] this->data;
			this->data = values;
		}
	}
public:
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

	T & operator[](size_t i)
	{
		if (i >= this->size)
		{
			this->expand(i);
		}
		return this->data[i];
	}

	const T & Root() const
	{
		if (this->size)
		{
			this->current = 0;
			return this->data[0];
		}
		throw std::exception("No root in tree");
	}

	const T & Current() const
	{
		if (this->size)
		{
			size_t c = this->current;
			this->current = 0;
			return this->data[c];
		}
		throw std::exception("No root in tree");
	}

	const T & Get() const
	{
		if (this->size)
		{
			return this->data[this->current];
		}
		throw std::exception("No root in tree");
	}

	DataVect & Left()
	{
		return *this;
	}

	DataVect & Right()
	{
		return *this;
	}

	DataVect & Parent()
	{
		if (this->current == 0)
		{
			return *this;
		}
		this->current = (this->current - 1) / 2;
		return *this;
	}
};

// T is the type of data and Comparator is a callable object that returns value < 0 if a < b, value > 0 if a > b and value == 0 if a == b
template<typename T, typename Comparator>
class VectBinTree
{
private:
	DataVect<T> data;
	size_t current;
public:
	VectBinTree();
	bool Empty() const;

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
