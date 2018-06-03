#include "Polynom.h"


template<typename T>
Polynom<T>::Polynom()
{
}

template<typename T>
Polynom<T>::~Polynom()
{
}

//template<typename T>
//bool Polynom<T>::operator==(const Polynom & other) const
//{
//	throw "not implemented";
//	return false;
//}

template<typename T>
inline bool operator==(const Polynom<T> & l, const Polynom<T> & r)
{
	return l.getDeg() == r.getDeg();
}

template<typename T>
inline bool operator!=(const Polynom<T>& l, const Polynom<T>& r)
{
	return !(l == r);
}

template<typename T>
inline bool operator<(const Polynom<T>& l, const Polynom<T>& r)
{
	return l.getDeg() < r.getDeg();
}

template<typename T>
inline bool operator>(const Polynom<T>& l, const Polynom<T>& r)
{
	return r < l;
}

template<typename T>
inline bool operator<=(const Polynom<T>& l, const Polynom<T>& r)
{
	return (l < r) || l == r;
}

template<typename T>
inline bool operator>=(const Polynom<T>& l, const Polynom<T>& r)
{
	return r <= l;
}

template<typename T>
Polynom<T>& operator+(const Polynom<T>& l, const Polynom<T>& r)
{
	// todo
}
