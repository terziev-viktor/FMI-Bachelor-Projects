#pragma once
#include "../../HelpersAndTools/HelpersAndTools/DataVector.hpp"
#include <functional>
#include <iostream>
using std::cout;
using std::cin;

template<typename Key, typename T, class HashingFunction = std::hash<T>>
class Hash
{
public:
	struct HashValuesListNode
	{
		T value;
		HashValuesListNode * next;
	};
private:
	HashingFunction hashingFunction;
	DataVector<HashValuesListNode> data;
public:
	Hash();
	~Hash();
	void Map(const T & value)
	{
		size_t hsh = hashingFunction(value);
		cout << hsh << endl;
	}
};

template<typename Key, typename T, class HashingFunction>
inline Hash<Key, T, HashingFunction>::Hash()
{
}

template<typename Key, typename T, class HashingFunction>
inline Hash<Key, T, HashingFunction>::~Hash()
{
}
