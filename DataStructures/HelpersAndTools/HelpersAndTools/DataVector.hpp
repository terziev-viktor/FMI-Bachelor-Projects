#pragma once
template<typename T>
class DataVector
{
private:
	struct TValue
	{
		bool free;
		T value;
	};
	TValue * data;
	size_t size;

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
	DataVector()
		:data(nullptr), size(0)
	{

	}

	~DataVector()
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

	const T & Get(size_t i) const
	{
		if (i >= this->size)
		{
			this->expand(i);
		}
		return this->data[i].value;
	}

	void Set(size_t i, const T & value)
	{
		if (i >= this->size)
		{
			this->expand(i);
		}
		this->data[i].value = value;
		this->data[i].free = false;
	}

	// Returns whenter the place is taken or not
	// true means taken and false means free to store data
	bool operator[](size_t i) const
	{
		if (i >= this->size)
		{
			this->expand(i);
		}
		return !this->data[i].free;
	}
};
