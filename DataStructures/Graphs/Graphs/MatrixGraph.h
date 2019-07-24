#pragma once
#include <array>
using std::array;
template<typename T>
class Vertex
{
	T d;
	bool free;
};

template<class T>
class MatrixGraph
{
private:
	const size_t INITSIZE = 100;
	array<Vertex, INITSIZE> data;
public:
};

template<class T>
inline MatrixGraph<T>::MatrixGraph()
{
	
}
