#pragma once
#include <iostream>

template<typename T>
class Polynom
{
public:
	Polynom();
	~Polynom();
	//const int getDeg() const;

	// comparrison
	friend bool operator==(const Polynom<T> & l, const Polynom<T> & r);
	friend bool operator!=(const Polynom<T> & l, const Polynom<T> & r);
	friend bool operator<(const Polynom<T> & l, const Polynom<T> & r);
	friend bool operator>(const Polynom<T> & l, const Polynom<T> & r);
	friend bool operator<=(const Polynom<T> & l, const Polynom<T> & r);
	friend bool operator>=(const Polynom<T> & l, const Polynom<T> & r);

	//friend Polynom<T> & operator+(const Polynom<T> & l, const Polynom<T> & r);
	//friend Polynom<T> & operator-(const Polynom<T> & l, const Polynom<T> & r);
	//friend Polynom<T> & operator*(const Polynom<T> & l, const Polynom<T> & r);
	//friend Polynom<T> & operator/(const Polynom<T> & l, const Polynom<T> & r);
	//friend Polynom<T> & operator%(const Polynom<T> & l, const Polynom<T> & r);

	//Polynom<T> & operator+=(const Polynom<T> & other);
	//Polynom<T> & operator-=(const Polynom<T> & other);
	//Polynom<T> & operator*=(const Polynom<T> & other);
	//Polynom<T> & operator/=(const Polynom<T> & other);
	//Polynom<T> & operator%=(const Polynom<T> & other);

	//// returns Integral of this polynom
	//Polynom<T> & operator++();

	//// Returns the derivative of this polynom
	//Polynom<T> & operator--();

	//// returns the degree of index i
	//int operator[](const int i);

	//// calculates value in point x
	//T & operator()(const T & x);

	//// calculates Integral from a to b
	//T & operator(const T & a, const T & b);

	//friend std::ostream & operator<<(std::ostream & os, const Polynom<T> & poly);
	//friend std::istream & operator>>(std::istream & in, Polynom<T> & poly);

private:
//	Vector<T> coef; // коефициенти
//	Vector<int> degrees; // степени
	int deg;
};

