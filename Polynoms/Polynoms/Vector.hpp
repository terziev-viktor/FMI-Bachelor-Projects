#pragma once
#include <iostream>
#include <math.h>

// Vector of elements of type <T>. The vector represents a polynom over a field of elements of type <T>

template<class T> class Vector
{
public:
	Vector();
	Vector(const unsigned int capacity);
	Vector(const Vector<T> & other);
	~Vector();

	static const unsigned int INIT_SIZE = 15;

	// Prints a string representation of the vector on stream <os>
	void print(std::ostream & os) const;

	// Returns element at positon <at>
	const T & getAt(unsigned int at) const;

	// Adds <item> after the last not zero element of the vector
	void push_back(const T & item);

	// Returns the first element of the vector
	const T & front() const;

	// Returns the last not zero element of the vector
	const T & back() const;

	// Gives read-only access to vector's elements by index
	const T & operator[] (unsigned int index) const;

	// Gives read-write access to vector's elements by index.
	// Note that setting the senior coefficient to 0 will 'break' the vector as the real degree will be decreased by 1 so I would not implement this operator.
	T & operator[] (unsigned int index);

	// Returns the biggest element in the vector
	const T & max() const;

	// Returns whether <item> exists in the vector
	// <out> will contain <item> index if <item> exists in the vector
	bool contains(const T & item, unsigned int & out) const;

	// Returns whether <item> exists in the vector
	bool contains(const T & item) const;

	// Removes element at position <index>
	// Throws std::out_of_range exception if <index> is invalid
	void removeAt(unsigned int index);

	// Returns whether no items have been added to the vector
	const bool isEmpty() const;

	// Removes all elements of the vector
	void clear();

	void clearEndingZeros();

	// Returns number of added elements to the vector
	const unsigned int count() const;

	Vector<T> & operator=(const Vector<T> & other);
	Vector<T> & operator*=(const Vector<T> & other);
	Vector<T> & operator/=(const Vector<T> & right);
	Vector<T> & operator%=(const Vector<T> & right);

	Vector<T> & operator-=(const Vector<T> other);
	Vector<T> & operator-=(const T & scalar);
	Vector<T> & operator+=(const Vector<T> other);
	Vector<T> & operator+=(const T & scalar);

	// Scalar multiplication
	Vector<T> & operator*=(const T & scalar);
	// Scalar division
	Vector<T> & operator/=(const T & scalar);

	// Returns value of polynom in point <x> represented by the vector
	T operator()(const T & x);

	// Returns Integral from <a> to <b> of the polynom represented by the vector
	T operator()(const T & a, const T & b);

	// Operator for casting to bool. Returns whether the vector is empty
	operator bool();

	bool operator!();
	operator unsigned int();
	operator const unsigned int();

	Vector<T> & operator++();
	const Vector<T> operator++(int);

	Vector<T> & operator--();
	const Vector<T> operator--(int);

	// Vector<T>'s iterator.
	// Gives access to elements in the vector without exposing its internal structure
	class VectorIterator
	{
	private:
		T * front; // start of data array
		T * current; // position of pointer to data 
		unsigned int size; // size of data array
		T * end; // end of data array
	public:
		VectorIterator(T * start, unsigned int size)
		{
			this->front = start;
			this->current = start;
			this->size = size;
			this->end = start + size;
		}
		VectorIterator(const VectorIterator & other)
		{
			this->front = other.front;
			this->current = other.current;
			this->size = other.size;
			this->end = other.end;
		}
		const T * first() const
		{
			return this->front;
		}
		const T * last() const
		{
			return this->last;
		}
		void next()
		{
			++this->current;
		}
		
		bool hasNext() const
		{
			return this->current < this->last;
		}
		bool isDone() const
		{
			return this->current == this->end;
		}
		const VectorIterator & operator++(int)
		{
			++this->current;
			return *this;
		}
		VectorIterator & operator++()
		{
			VectorIterator tmp = *this;
			++this->current;
			return tmp;
		}

		const VectorIterator & operator--(int)
		{
			VectorIterator tmp = *this;
			--this->current;
			return tmp;
		}
		VectorIterator & operator--()
		{
			--this->current;
			return *this;
		}
		T * operator->()
		{
			return this->current;
		}
		T & operator*()
		{
			return *this->current;	
		}
		bool operator<(int i)
		{
			return current < (this->front + i);
		}
		bool operator>(int i)
		{
			return current > (this->front + i);
		}
		bool operator==(int i)
		{
			return current == (this->front + i);
		}
		bool operator>=(int i)
		{
			return current >= (this->front + i);
		}
		bool operator<=(int i)
		{
			return current <= (this->front + i);
		}
	};

	VectorIterator createIterator()
	{
		VectorIterator i(this->buffer, this->index);
		return i;
	}
private:
	unsigned int index;
	unsigned int size; // size of buffer
	T * buffer;
	// Sets <value> at position <at>
	// Throws out_of_range exception if argument <at> is invalid
	// This function expands the capacity of the vector as much as needed so use with caution. This the main reason why it is private
	void setAt(unsigned int at, const T & value);

	// Doubles buffer's size
	void expand();
	
	// Expands buffer's size to <to>
	void expand(unsigned int to);

	void setToZero();
};

template<class T>
inline void Vector<T>::push_back(const T & item)
{
	this->clearEndingZeros();
	if (this->index == this->size)
	{
		this->expand();
	}
	if (item == 0)
	{
		return;
	}
	this->buffer[this->index] = item;
	++this->index;
}

template<class T>
inline const unsigned int Vector<T>::count() const
{
	return this->index;
}

template<class T>
inline void Vector<T>::setAt(unsigned int at, const T & value)
{
	if (at < 0)
	{
		throw std::out_of_range("Index <at> is out of range.");
	}
	if (at >= this->index)
	{
		this->expand(at + 1);
	}
	this->buffer[at] = value;
	if (at >= this->index)
	{
		this->index = at + 1;
	}
}

template<class T>
inline const T & Vector<T>::operator[](unsigned int index) const
{
	// no idea how should I implement this.
	// If I assume the the vector is endless then I should return zero ?
	if (index < 0)
	{
		throw std::out_of_range("Index out of range.");
	}
	if (index >= this->index)
	{
		T t = 0;
		return t;
	}
	return this->buffer[index];
}

template<class T>
inline T & Vector<T>::operator[](unsigned int index)
{
	if (index < 0)
	{
		throw std::out_of_range("Index out of range.");
	}
	if (index >= this->index)
	{
		T t = 0;
		return t;
	}
	return this->buffer[index];
}

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
inline const T & Vector<T>::getAt(unsigned int at) const
{
	if (at < 0 || at >= this->index)
	{
		throw std::out_of_range("Index out of range.");
	}
	return this->buffer[at];
}

template<class T>
inline const T & Vector<T>::front() const
{
	return this->buffer[0];
}

template<class T>
inline const T & Vector<T>::back() const
{
	return this->buffer[this->index - 1];
}

template<class T>
inline const bool Vector<T>::isEmpty() const
{
	return this->index == 0;
}

template<class T>
inline void Vector<T>::clear()
{
	this->index = 0;
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
inline void Vector<T>::removeAt(unsigned int index)
{
	this->clearEndingZeros();
	if (index < 0)
	{
		throw std::out_of_range("Invalid index.");
	}
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
inline Vector<T> operator*(const Vector<T> & l, const T & scalar)
{
	l.clearEndingZeros();
	Vector<T> result(l.count());
	for (unsigned int i = 0; i < l.count(); i++)
	{
		T tmp = l[i];
		tmp *= scalar;
		result.push_back(tmp);
	}
	return result;
}

template<class T>
Vector<T> operator*(const Vector<T> & l, const Vector<T> & r)
{
	Vector<T> result = l;
	result *= r;
	return result;
}

template<class T>
Vector<T> operator/(const Vector<T> & l, const Vector<T> & r)
{
	Vector<T> result = l;
	result /= r;
	return result;
}

template<class T>
Vector<T> operator%(const Vector<T> & l, const Vector<T> & r)
{
	Vector<T> result = l;
	result %= r;
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
		if (i < l.count() && i < r.count())
		{
			sum = l[i] + r[i];
		}
		else if (i < r.count())
		{
			sum = r[i];
		}
		else
		{
			sum = l[i];
		}
		result.push_back(sum);
	}
	result.clearEndingZeros();
	return result;
}

template<class T>
Vector<T> operator-(const Vector<T> & l, const Vector<T> & r)
{
	if (&l == &r)
	{
		Vector<T> result(1);
		return result;
	}
	const unsigned int size = l.count() > r.count() ? l.count() : r.count();
	Vector<T> result(size);
	T diff;
	for (unsigned int i = 0; i < size; i++)
	{
		if (i < l.count() && i < r.count())
		{
			diff = l[i] - r[i];
		}
		else if (i < r.count())
		{
			diff = r[i] * -1;
		}
		else
		{
			diff = l[i];
		}
		result.push_back(diff);
	}
	result.clearEndingZeros();
	return result;
}


template<class T>
bool operator==(const Vector<T> & l, const Vector<T> & r)
{
	if (l.count() != r.count())
	{
		return false;
	}
	for (unsigned int i = 0; i < l.count(); i++)
	{
		if (l[i] != r[i])
		{
			return false;
		}
	}
	return true;
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
		for (unsigned int i = 1; i <= n; ++i)
		{
			sum = sum + (temp[i] * other[n - i]);
		}
		this->push_back(sum);
	}
	this->clearEndingZeros();
	return *this;
}

template<class T>
inline Vector<T>& Vector<T>::operator/=(const Vector<T>& right)
{
	if (this->count() == 0 || right.count() == 0)
	{
		throw "Logical exception";
	}
	unsigned int leftDeg = this->count() - 1;
	unsigned int rightDeg = right.count() - 1;
	if (leftDeg < rightDeg)
	{
		// the result is the zero vector
		this->clear();
		this->push_back(0);
		return *this;
	}
	unsigned int resultdeg = leftDeg - rightDeg;
	unsigned int i = resultdeg;
	Vector<T> result(resultdeg + 1);

	while (leftDeg >= rightDeg)
	{
		const T leftSenior = this->back();
		const T rightSenior = right.back();
		const T resultSenior = leftSenior / rightSenior;
		Vector<T> tmp;
		tmp.setAt(i, resultSenior);
		cout << tmp << endl;
		result.setAt(i, resultSenior);
		--i;
		Vector<T> multi = right * tmp;
		cout << multi << endl;
		*this -= multi;
		if (this->count() == 0)
		{
			leftDeg = 0;
		}
		else {
			leftDeg = this->count() - 1;
		}
	}
	*this = result;
	return *this;
}

template<class T>
inline Vector<T>& Vector<T>::operator%=(const Vector<T>& right)
{
	if (this->count() == 0 || right.count() == 0)
	{
		throw "Logical exception";
	}
	unsigned int leftDeg = this->count() - 1;
	unsigned int rightDeg = right.count() - 1;
	if (leftDeg < rightDeg)
	{
		// the result of the devision is 0 + the left vector (this)
		return *this;
	}
	unsigned int resultdeg = leftDeg - rightDeg;
	unsigned int i = resultdeg;

	while (leftDeg >= rightDeg)
	{
		const T leftSenior = this->back();
		const T rightSenior = right.back();
		const T resultSenior = leftSenior / rightSenior;
		Vector<T> tmp;
		tmp.setAt(i, resultSenior);
		cout << tmp << endl;
		--i;
		Vector<T> multi = right * tmp;
		cout << multi << endl;
		*this -= multi;
		if (this->count() == 0)
		{
			leftDeg = 0;
		}
		else 
		{
			leftDeg = this->count() - 1;
		}
	}
	return *this;
}

template<class T>
inline Vector<T>& Vector<T>::operator-=(const Vector<T> other)
{
	*this = *this - other;
	return *this;
}

template<class T>
inline Vector<T>& Vector<T>::operator-=(const T & scalar)
{
	if (this->size == 0)
	{
		this->expand(Vector<T>::INIT_SIZE);
	}
	this->buffer[0] -= scalar;
	return *this;
}

template<class T>
inline Vector<T>& Vector<T>::operator+=(const Vector<T> other)
{
	*this = *this + other;
	return *this;
}

template<class T>
inline Vector<T>& Vector<T>::operator+=(const T & scalar)
{
	if (this->size == 0)
	{
		this->expand(Vector<T>::INIT_SIZE);
	}
	this->buffer[0] += scalar;
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
	T current;
	for (unsigned int i = 1; i < this->index; i++)
	{
		current = this->buffer[i];
		T xPowerValue = pow(x, i);
		current *= xPowerValue;
		result += current;
	}
	return result;
}

template<class T>
inline T Vector<T>::operator()(const T & a, const T & b)
{
	Vector<T> F = (*this);
	F++;
	T left = F(a);
	T right = F(b);

	return left - right;
}

template<class T>
inline Vector<T>::operator bool()
{
	return this->count() != 0;
}

template<class T>
inline bool Vector<T>::operator!()
{
	return this->count() == 0;
}

template<class T>
inline Vector<T>::operator unsigned int()
{
	return this->index - 1;
}

template<class T>
inline Vector<T>::operator const unsigned int()
{
	return this->index - 1;
}

template<class T>
inline Vector<T>& Vector<T>::operator++()
{
	this->clearEndingZeros();
	this->push_back(0);
	for (unsigned int i = this->index - 1; i > 0; i--)
	{
		T coef = this->buffer[i - 1];
		coef /= (T)i;
		this->buffer[i] = coef;
	}
	this->buffer[0] = (T)0;

	return *this;
}

template<class T>
inline const Vector<T> Vector<T>::operator++(int)
{
	this->clearEndingZeros();
	Vector<T> old = *this;
	this->push_back(0);
	for (unsigned int i = this->index - 1; i > 0; i--)
	{
		T coef = this->buffer[i - 1];
		coef /= (T)i;
		this->buffer[i] = coef;
	}
	this->buffer[0] = (T)0;
	return old;
}

template<class T>
inline Vector<T>& Vector<T>::operator--()
{
	this->clearEndingZeros();
	for (unsigned int i = 0; i < this->index - 1; i++)
	{
		T coef = this->buffer[i + 1];
		coef *= (T)(i + 1);
		this->buffer[i] = coef;
	}
	--this->index;
	return *this;
}

template<class T>
inline const Vector<T> Vector<T>::operator--(int)
{
	this->clearEndingZeros();
	Vector<T> old = *this;
	for (unsigned int i = 0; i < this->index - 1; i++)
	{
		T coef = this->buffer[i + 1];
		coef *= (T)(i + 1);
		this->buffer[i] = coef;
	}
	--this->index;
	return old;
}

template<class T>
inline Vector<T>::Vector()
{
	this->size = Vector::INIT_SIZE;
	this->index = 0;
	this->buffer = new T[this->size];
	this->setToZero();
}

template<class T>
inline Vector<T>::Vector(const unsigned int capacity)
{
	if (capacity <= 0)
	{
		this->size = 1;
	}
	else
	{
		this->size = capacity;
	}
	this->index = 0;
	this->buffer = new T[this->size];
	this->setToZero();
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
inline void Vector<T>::clearEndingZeros()
{
	while (this->buffer[this->index - 1] == 0 && this->index > 0)
	{
		--this->index;
	}
}

template<class T>
inline Vector<T>& Vector<T>::operator=(const Vector<T>& other)
{
	if (this != &other)
	{
		if (this->buffer)
		{
			delete[] this->buffer;
		}
		this->buffer = new T[other.index];
		for (unsigned int i = 0; i < other.index; i++)
		{
			this->buffer[i] = other.buffer[i];
		}
		this->size = other.index;
		this->index = other.index;
	}
	return *this;
}

template<class T>
inline void Vector<T>::expand()
{
	this->clearEndingZeros();
	this->size *= 2;
	T * biggerBuffer = new T[this->size];
	for (unsigned int i = 0; i < this->index; i++)
	{
		biggerBuffer[i] = this->buffer[i];
	}
	for (unsigned int i = this->index; i < this->size; i++)
	{
		biggerBuffer[i] = 0;
	}
	delete[] this->buffer;
	this->buffer = biggerBuffer;
}

template<class T>
inline void Vector<T>::expand(unsigned int to)
{
	this->clearEndingZeros();
	this->size = to;
	T * biggerBuffer = new T[this->size];
	for (unsigned int i = 0; i < this->index; i++)
	{
		biggerBuffer[i] = this->buffer[i];
	}
	for (unsigned int i = this->index; i < this->size; i++)
	{
		biggerBuffer[i] = 0;
	}
	delete[] this->buffer;
	this->buffer = biggerBuffer;
}

template<class T>
inline void Vector<T>::setToZero()
{
	for (unsigned i = 0; i < this->size; i++)
	{
		this->buffer[i] = 0;
	}
}

template<class T>
bool operator<(const Vector<T> & left, const Vector<T> & right)
{
	if (&left == &right)
	{
		return false;
	}
	left.clearEndingZeros();
	right.clearEndingZeros();
	return left.count() < right.count();
}

template<class T>
bool operator>(const Vector<T> & left, const Vector<T> & right)
{
	if (&left == &right)
	{
		return false;
	}
	left.clearEndingZeros();
	right.clearEndingZeros();
	return left.count() > right.count();
}

template<class T>
bool operator<=(const Vector<T> & left, const Vector<T> & right)
{
	if (&left == &right)
	{
		return true;
	}
	left.clearEndingZeros();
	right.clearEndingZeros();
	return left.count() <= right.count();
}

template<class T>
bool operator>=(const Vector<T> & left, const Vector<T> & right)
{
	if (&left == &right)
	{
		return true;
	}
	left.clearEndingZeros();
	right.clearEndingZeros();
	return left.count() >= right.count();
}

template<class T>
std::ostream & operator<<(std::ostream & os, const Vector<T> & vector)
{
	os << '(';
	if (vector.count() == 0)
	{
		os << ')';
		return os;
	}
	for (unsigned int i = 0; i < vector.count() - 1; i++)
	{
		os << vector[i] << ", ";
	}
	os << vector[vector.count() - 1] << ")";
	return os;
}
