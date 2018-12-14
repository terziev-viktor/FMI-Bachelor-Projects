#pragma once

#include "LinkedList.hpp"

// A Hash Table with user-defined HasingFunction that gets a Key and returns a hash of type size_t
template<typename Key, typename Value, size_t(*HashingFunction)(const Key &)>
class HashTable
{
public:
	class KeyValuePair
	{
	protected:
		Key key;
		Value value;
	public:
		const Value & GetValue() const { return this->value; }

		Value & GetValue() { return this->value; }

		const Key & GetKey() const { return this->key; }

		void SetValue(const Value & v)
		{
			this->value = v;
		}

		KeyValuePair()
		{	}

		KeyValuePair(const Key & k, const Value & v)
		{
			this->key = k;
			this->SetValue(v);
		}
	};
protected:
	bool expandWhenFull; // the user says if he wants to expand the table when its full

	size_t count; // count of elements in the table, including duplicate hashes

	size_t space; // count of used lists in the table

	size_t size; // size of the table

	void expand()
	{
		size_t n_size = this->size * 2;
		size_t n_space = n_size;
		LinkedList<KeyValuePair> * n_data = new LinkedList<KeyValuePair>[n_size];
		for (Iterator it = this->Begin(); !it.Done(); ++it)
		{
			size_t hsh = HashingFunction((*it)->GetKey()) % n_size;
			if (n_data[hsh].Empty())
			{
				--n_space;
			}

			n_data[hsh].AddFront(KeyValuePair((*it)->GetKey(), (*it)->GetValue()));
		}
		this->size = n_size;
		this->space = n_space;
		delete[] this->data;
		this->data = n_data;
	}

	LinkedList<KeyValuePair> * data;

public:
	
	HashTable(const size_t INIT_SIZE = 100, bool expandWhenFull = false);

	~HashTable();

	class Iterator
	{
	private:
		typename LinkedList<KeyValuePair>::Iterator it;
		LinkedList<KeyValuePair> * data;
		size_t size;
	public:
		Iterator()
			:data(nullptr), size(0)
		{

		}

		Iterator(LinkedList<KeyValuePair> * begining, size_t size)
		{
			this->data = begining;
			this->size = size;	
			while (this->data->Empty())
			{
				++data;
				--this->size;
			}
			it = this->data->Begin();
		}

		bool Done() const
		{
			return this->size == 0;
		}

		Iterator & operator++()
		{
			++it;
			if (size && it.Done())
			{
				++this->data;
				--this->size;
				while (size && this->data->Empty())
				{
					++data;
					--size;
				}
				this->it = this->data->Begin();
			}
			return *this;
		}

		Iterator & operator++(int)
		{
			Iterator old(*this);

			++it;
			if (size && it.Done())
			{
				++data;
				--size;
				while (size && this->data->Empty())
				{
					++data;
					--size;
				}
				this->it = this->data->Begin();
			}

			return old;
		}

		const KeyValuePair * operator*() const
		{
			return &(*it);
		}

		KeyValuePair * operator*()
		{
			return &(*it);
		}

		Iterator * operator->()
		{
			return this;
		}
	};

	Iterator Begin() const
	{
		return Iterator(this->data, this->size);
	}

	// Density of the values in the table
	// The closer to 1.0 ther better
	double Density() const
	{
		return (double)this->Count() / this->Size();
	}

	// Distribution of values in the table - occupied space over total size.
	// The closer to 1.0 the better
	double Distribution() const
	{
		return (double)(this->Size() - this->FreeSpace()) / this->Size();
	}

	// Returns the space left in the table not including duplicate hashes
	// Not same as count of elements
	size_t FreeSpace() const
	{
		return this->space;
	}

	// Returns the size of the table
	size_t Size() const
	{
		return this->size;
	}

	// Returns the count of elements inserted in the table
	size_t Count() const
	{
		return this->count;
	}

	// Inserts Key-Value pair in the table
	// Speed: pretty much O(1)
	bool Insert(const Key & key, const Value & value);

	// Deletes the value of given key
	void Delete(const Key & key);

	// Finds and returns a pointer to the value at given key
	// Speed: pretty much O(1)
	Value * Search(const Key & key);

	// Performs a Search, Added for readability
	Value * operator[](const Key & key);

	KeyValuePair * GetKeyValuePairByKey(const Key & key)
	{
		size_t hsh = HashingFunction(key) % this->size;
		if (!this->data[hsh].Empty())
		{
			if (this->data[hsh].Size() == 1)
			{
				return &this->data[hsh].GetFirst();
			}
			for (typename LinkedList<KeyValuePair>::Iterator it = this->data[hsh].Begin(); !it.Done(); ++it)
			{
				if ((*it).GetKey() == key)
				{
					return &(*it);
				}
			}
		}
		return nullptr;
	}

	// Doubles the capacity of the table. Use with cotion!
	// Speed: A very slow O(n) because of deletions and constructions of objects
	void Expand()
	{
		this->expand();
	}

	// Iterates through all elements and prints them
	// Speed: O(n) where n is the size of the table
	void Print(std::ostream & out) const;
};

template<typename Key, typename Value, size_t(*HashingFunction)(const Key &)>
inline HashTable<Key, Value, HashingFunction>::HashTable(const size_t INIT_SIZE, bool expandWhenFull)
{
	this->size = INIT_SIZE;
	this->space = this->size;
	this->expandWhenFull = expandWhenFull;
	this->data = new LinkedList<KeyValuePair>[this->size];
}

template<typename Key, typename Value, size_t(*HashingFunction)(const Key &)>
inline HashTable<Key, Value, HashingFunction>::~HashTable()
{
	delete[] this->data;
}

template<typename Key, typename Value, size_t(*HashingFunction)(const Key &)>
inline bool HashTable<Key, Value, HashingFunction>::Insert(const Key & key, const Value & value)
{
	if (this->space == 0 && expandWhenFull) // using all space in the table, time to expand
	{
		this->expand();
	}
	size_t hsh = HashingFunction(key) % this->size;
	if (this->data[hsh].Empty())
	{
		--this->space;
	}

	this->data[hsh].AddFront(KeyValuePair(key, value));
	++this->count;
	return true;
}

template<typename Key, typename Value, size_t(*HashingFunction)(const Key &)>
inline void HashTable<Key, Value, HashingFunction>::Delete(const Key & key)
{
	size_t h = HashingFunction(key) % this->size;
	if (!this->data[h].Empty())
	{
		for (typename LinkedList<KeyValuePair>::Iterator it = this->data[h].Begin(); !it.Done(); ++it)
		{
			if ((*it).GetKey() == key)
			{
				this->data[h].Remove(it);
				--count;
			}
		}
	}
	throw "No such key exists in the table";
}

template<typename Key, typename Value, size_t(*HashingFunction)(const Key &)>
inline Value * HashTable<Key, Value, HashingFunction>::Search(const Key & key)
{
	return &this->GetKeyValuePairByKey(key)->GetValue();
}

template<typename Key, typename Value, size_t(*HashingFunction)(const Key &)>
inline Value * HashTable<Key, Value, HashingFunction>::operator[](const Key & key)
{
	return this->Search(key);
}

template<typename Key, typename Value, size_t(*HashingFunction)(const Key &)>
inline void HashTable<Key, Value, HashingFunction>::Print(std::ostream & out) const
{
	for (HashTable::Iterator it = this->Begin(); !it->Done(); ++it)
	{
		out << "Key: " << (*it)->GetKey() << ", Value: " << (*it)->GetValue() << '\n';
	}
}
