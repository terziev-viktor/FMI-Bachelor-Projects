#pragma once
#include <iostream>
#include <math.h>
// Vector of elements of type T
// T should have operator= implemented
template<class T>
class Vector
{
public:
	Vector();
	Vector(const unsigned int capacity);
	Vector(const Vector<T> & other);
	~Vector();

	static const unsigned int INIT_SIZE = 15;
	void print(std::ostream & os) const;
	void setAt(unsigned int at, const T & value);

	void push_back(const T & item);
	const T & front() const;
	const T & back() const;
	const T & operator[] (unsigned int index) const;
	T operator[] (unsigned int index);

	const T & max() const;
	bool contains(const T & item, unsigned int & out) const;
	bool contains(const T & item) const;

	void removeAt(unsigned int index);
	const bool isEmplty() const;
	void clear();
	void clearEndingZeros();

	const unsigned int count() const;

	Vector<T> & operator=(const Vector<T> & other);
	bool operator==(const Vector<T> & other);
	Vector<T> & operator+=(const Vector<T> & other);
	Vector<T> & operator*=(const Vector<T> & other);
	Vector<T> & operator*=(const T & scalar);
	Vector<T> & operator/=(const T & scalar);
	T operator()(const T & x);
	// Integral from a to b
	T operator()(const T & a, const T & b);
	operator bool();
	operator const T&();
	Vector<T> & operator++();
	const Vector<T> operator++(T);

private:
	unsigned int index;
	unsigned int size;
	T * buffer;
	void expand();
};

template<class T>
inline void Vector<T>::print(std::ostream & os) const
{
	os << "(";
	for (unsigned int i = 0; i < this->index - 1; i++)
	{
		os << this->buffer[i] << ", ";
	}
	os << this->buffer[this->index - 1] << ")";
}

template<class T>
inline void Vector<T>::setAt(unsigned int at, const T & value)
{
	if (at >= 0 && at < this->index)
	{
		this->buffer[at] = value;
	}
}

template<class T>
Vector<T> operator*(const Vector<T> & l, const Vector<T> & r)
{
	const unsigned int deg = (l.count() - 1) * (r.count() - 1);
	Vector<T> result(deg);
	for (unsigned int n = 0; n <= deg; ++n)
	{
		T sum = l[0] * r[n];
		for (unsigned int i = 1; i <= n; i++)
		{
			T left = l[i];
			T rig = r[n - i];
			T diff = left * rig;
			sum += diff;
		}
		result.push_back(sum);
	}
	result.clearEndingZeros();
	return result;
}

template<class T>
Vector<T> operator+(const Vector<T> & l, const Vector<T> & r)
{
	const unsigned int size = l.count() > r.count() ? l.count() : r.count();
	Vector<T> result(size);
	T sum;
	for (unsigned int i = 0; i < size; i++)
	{
		sum = l[i] + r[i];
		result.push_back(sum);
	}
	result.clearEndingZeros();
	return result;
}

template<class T>
Vector<T> operator-(const Vector<T> & l, const Vector<T> & r)
{
	const unsigned int size = l.count() > r.count() ? l.count() : r.count();
	Vector<T> result(size);
	T sum;
	for (unsigned int i = 0; i < size; i++)
	{
		sum = l[i] - r[i];
		result.push_back(sum);
	}
	result.clearEndingZeros();
	return result;
}

template<class T>
std::ostream & operator<<(std::ostream & os, const Vector<T> & vector)
{
	os << "(";
	for (unsigned int i = 0; i < vector.count() - 1; i++)
	{
		os << vector[i] << ", ";
	}
	os << vector[vector.count() - 1] << ")";
	return os;
}

template<class T>
inline bool Vector<T>::operator==(const Vector<T> & other)
{
	if (this->count() != other.count())
	{
		return false;
	}
	for (unsigned int i = 0; i < other.count(); i++)
	{
		if (this->buffer[i] != other.buffer[i])
		{
			return false;
		}
	}
	return true;
}

template<class T>
inline Vector<T>& Vector<T>::operator+=(const Vector<T>& other)
{
	Vector<T> temp = *this;
	const unsigned int count = this->count() > other.count() ? this->count() : other.count();
	this->clear();

	for (unsigned int i = 0; i < count; i++)
	{
		this->push_back(temp[i] + other[i]);
	}
	this->clearEndingZeros();
	return *this;
}

template<class T>
inline Vector<T>& Vector<T>::operator*=(const Vector<T>& other)
{
	Vector<T> temp = *this;
	//degree of a polynom is count of coefficients - 1 ( (1, 2, 3) == 1 + 2x + 3x^2 is with degree of 2 but it has 3 elements)
	unsigned int deg = (this->count() - 1) + (other.count() - 1);
	this->clear();
	for (unsigned int n = 0; n <= deg; ++n)
	{
		T sum = temp[0] * other[n];
		for (unsigned int i = 1; i <= n; i++)
		{
			sum = sum + (temp[i] * other[n - i]);
		}
		this->push_back(sum);
	}
	this->clearEndingZeros();
	return *this;
}

template<class T>
inline Vector<T>& Vector<T>::operator*=(const T & scalar)
{
	T tmp;
	for (unsigned int i = 0; i < this->count(); i++)
	{
		tmp = this->buffer[i];
		tmp *= scalar;
		this->setAt(i, tmp);
	}
	return *this;
}

template<class T>
inline Vector<T>& Vector<T>::operator/=(const T & scalar)
{
	T tmp;
	for (unsigned int i = 0; i < this->count(); i++)
	{
		tmp = this->buffer[i];
		tmp /= scalar;
		this->setAt(i, tmp);
	}
	return *this;
}

template<class T>
inline T Vector<T>::operator()(const T & x)
{
	T result = this->buffer[0];
	for (unsigned int i = 1; i < this->index; i++)
	{
		result += this->buffer[i] * pow(x, i);
	}
	return result;
}

template<class T>
inline T Vector<T>::operator()(const T & a, const T & b)
{
	throw "not implemented";
	return T();
}

template<class T>
inline Vector<T>::operator bool()
{
	return this->count() != 0;
}

template<class T>
inline Vector<T>::operator const T&()
{
	return this->front();
}

template<class T>
inline Vector<T>& Vector<T>::operator++()
{
	// todo
}

template<class T>
inline const Vector<T> Vector<T>::operator++(T)
{
	return Vector<T>();
}

template<class T>
inline const T & Vector<T>::operator[](unsigned int index) const
{
	if (index >= this->index)
	{
		return 0;
	}
	return this->buffer[index];
}

template<class T>
inline T Vector<T>::operator[](unsigned int index)
{
	if (index >= this->index)
	{
		return 0;
	}
	return this->buffer[index];
}

template<class T>
inline const T & Vector<T>::max() const
{
	T max = this->buffer[0];
	for (unsigned int i = 1; i < this->index; i++)
	{
		if (max < this->buffer[i])
		{
			max = this->buffer[i];
		}
	}
	return max;
}

template<class T>
inline bool Vector<T>::contains(const T & item, unsigned int & out) const
{
	for (unsigned int i = 0; i < this->index; i++)
	{
		if (item == buffer[i])
		{
			out = i;
			return true;
		}
	}
	return false;
}

template<class T>
inline bool Vector<T>::contains(const T & item) const
{
	for (unsigned int i = 0; i < this->index; i++)
	{
		if (item == buffer[i])
		{
			return true;
		}
	}
	return false;
}

template<class T>
inline Vector<T>::Vector()
{
	this->size = Vector::INIT_SIZE;
	this->index = 0;
	this->buffer = new T[this->size];
}

template<class T>
inline Vector<T>::Vector(const unsigned int capacity)
{
	this->size = capacity;
	this->index = 0;
	this->buffer = new T[this->size];
}

template<class T>
inline Vector<T>::Vector(const Vector<T>& other)
{
	this->size = other.size;
	this->index = other.index;
	this->buffer = new T[this->size];
	for (unsigned int i = 0; i < this->index; i++)
	{
		this->buffer[i] = other.buffer[i];
	}
}

template<class T>
inline Vector<T>::~Vector()
{
	if (this->buffer)
	{
		delete[] this->buffer;
	}
}

template<class T>
inline void Vector<T>::push_back(const T & item)
{
	if (this->index == this->size)
	{
		this->expand();
	}
	this->buffer[this->index] = item;
	++this->index;
}

template<class T>
inline const T & Vector<T>::front() const
{
	// todo: check for valid argument
	return this->buffer[0];
}

template<class T>
inline const T & Vector<T>::back() const
{
	return this->buffer[this->index - 1];
}

template<class T>
inline void Vector<T>::removeAt(unsigned int index)
{
	if (index == this->index - 1)
	{
		--this->index;
	}
	else if (index >= 0 && index < this->index - 1)
	{
		for (unsigned int i = index; i < this->index - 1; i++)
		{
			this->buffer[i] = this->buffer[i + 1];
		}
		--this->index;
	}
}

template<class T>
inline const bool Vector<T>::isEmplty() const
{
	return this->index == 0;
}

template<class T>
inline void Vector<T>::clear()
{
	this->index = 0;
}

template<class T>
inline void Vector<T>::clearEndingZeros()
{
	while (this->buffer[this->index - 1] == 0 && this->index > 0)
	{
		--this->index;
	}
}

template<class T>
inline const unsigned int Vector<T>::count() const
{
	return this->index;
}

template<class T>
inline Vector<T>& Vector<T>::operator=(const Vector<T>& other)
{
	if (this->buffer)
	{
		delete[] this->buffer;
	}
	this->buffer = new T[other->size];
	for (unsigned int i = 0; i < other.index; i++)
	{
		this->buffer[i] = other.buffer[i];
	}
	this->index = other.index;
	this->size = other.size;

	return *this;
}

template<class T>
inline void Vector<T>::expand()
{
	this->size *= 2;
	T * biggerBuffer = new T[this->size];
	for (unsigned int i = 0; i < this->index; i++)
	{
		biggerBuffer[i] = this->buffer[i];
	}
	delete[] this->buffer;
	this->buffer = biggerBuffer;
}
