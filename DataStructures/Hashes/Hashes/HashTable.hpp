#pragma once
#include <functional>
#include <string>

// A Hash Table with user-defined HasingFunction that gets a Key and returns a hash of type size_t
template<typename Key, typename Value, size_t (*HashingFunction)(const Key &)>
class HashTable
{
protected:
	class KeyValueListNode
	{
	public:
		Key key;
		Value value;
		KeyValueListNode * next = nullptr;
		bool free = true;

		KeyValueListNode()
			:next(nullptr), free(true)
		{

		}

		~KeyValueListNode()
		{
			if (next)
			{
				delete next;
			}
		}

	};

	KeyValueListNode * data;

	bool expandWhenFull;

	KeyValueListNode * Find(const Key & key)
	{
		size_t hsh = HashingFunction(key) % this->size;
		if (this->data[hsh].key == key)
		{
			return &this->data[hsh];
		}
		else
		{
			KeyValueListNode * c = this->data[hsh].next;

			do
			{
				if (c->key == key)
				{
					return c;
				}
				c = c->next;
			} while (!c);
		}
		return nullptr;
	}

private:
	size_t count;

	size_t space;

	size_t size;

	void expand()
	{
		size_t n_size = this->size * 2;
		size_t n_space = n_size;
		KeyValueListNode * n_data = new KeyValueListNode[n_size];
		// Rehash all key-value pairs
		for (HashTable::Iterator it = this->Begin(); !it->Done(); ++it)
		{
			size_t n_hsh = HashingFunction((*it)->key) % n_size;
			if (n_data[n_hsh].free)
			{
				n_data[n_hsh].key = (*it)->key;
				n_data[n_hsh].value = (*it)->value;
				n_data[n_hsh].free = false;
				--n_space;
			}
			else
			{
				KeyValueListNode * c = n_data[n_hsh].next;
				KeyValueListNode * c_prev = &n_data[n_hsh];

				while (c != nullptr)
				{
					c_prev = c;
					c = c->next;
				}
				c = new KeyValueListNode();
				c->value = (*it)->value;
				c->key = (*it)->key;
				c_prev->next = c;
			}
		}
		this->size = n_size;
		this->space = n_space;
		delete[] this->data;
		this->data = n_data;
	}

public:

	HashTable(const size_t INIT_SIZE = 100, bool expandWhenFull = false);

	~HashTable()
	{
		delete[] this->data;
	}

	class Iterator
	{
	private:
		KeyValueListNode * c;
		KeyValueListNode * data;
		size_t size;
	public:
		Iterator()
			:c(nullptr),data(nullptr),size(0)
		{

		}
		Iterator(KeyValueListNode * begining, size_t size)
		{
			this->data = begining;
			size_t i = 0;
			// go to the first hashed key-value pair
			while (this->data->free)
			{
				--size;
				++data;
			}
			this->size = size;
			this->c = data;
		}

		bool Done() const
		{
			return this->size == 0;
		}

		Iterator & operator++()
		{
			if (c->next)
			{
				c = c->next;
			}
			else if(this->size)
			{
				++data;
				while (size && data->free)
				{
					++data;
					--size;
				}
				c = data;
			}
			return *this;
		}

		Iterator & operator++(int)
		{
			Iterator old(*this);

			if (c->next)
			{
				c = c->next;
			}
			else if (this->size)
			{
				++data;
				while (size && data->free)
				{
					++data;
					--size;
				}
				c = data;
			}
			return old;
		}

		const KeyValueListNode * operator*() const
		{
			return c;
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

	bool Delete(const Key & key);

	// Finds and returns a pointer to the value at given key
	// Speed: pretty much O(1)
	Value * Search(const Key & key);

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
	this->data = new KeyValueListNode[this->size];
}

template<typename Key, typename Value, size_t(*HashingFunction)(const Key &)>
inline bool HashTable<Key, Value, HashingFunction>::Insert(const Key & key, const Value & value)
{
	if (this->space == 0 && expandWhenFull) // using all space in the table, time to expand
	{
		this->expand();
	}
	size_t hsh = HashingFunction(key) % this->size;
	if (this->data[hsh].free)
	{
		this->data[hsh].value = value;
		this->data[hsh].key = key;
		this->data[hsh].free = false;
		--this->space;
		++this->count;
		return true;
	}
	else
	{
		KeyValueListNode * c = this->data[hsh].next;
		KeyValueListNode * c_prev = &this->data[hsh];
		
		while (c != nullptr)
		{
			c_prev = c;
			c = c->next;
		}
		c = new KeyValueListNode();
		c->value = value;
		c->key = key;
		c_prev->next = c;
		++this->count;
		return true;
	}
	return false;
}

template<typename Key, typename Value, size_t(*HashingFunction)(const Key &)>
inline bool HashTable<Key, Value, HashingFunction>::Delete(const Key & key)
{
	size_t h = HashingFunction(key) % this->size;
	if (this->data[h].key == key)
	{
		if (this->data[h].next == nullptr)
		{
			this->data[h].free = true;
			++space;
			--count;
			return true;
		}
		else
		{
			this->data[h].key = this->data[h].next->key;
			this->data[h].value = this->data[h].next->value;
			KeyValueListNode * n = this->data[h].next;
			this->data[h].next = this->data[h].next->next;
			n->next = nullptr;
			delete n;
			--count;
			return true;
		}
	}
	else
	{
		KeyValueListNode * c = this->data[h].next;
		while (c && c->key != key) c = c->next;
		if (!c) // no such key
		{
			return false;
		}
		if (!c->next)
		{
			// TODO
		}
		else
		{
			c->key = c->next->key;
			c->value = c->next->value;
			KeyValueListNode * n = c->next;
			c->next = c->next->next;
			n->next = nullptr;
			delete n;
			--count;
			return true;
		}
		
	}
}

template<typename Key, typename Value, size_t(*HashingFunction)(const Key &)>
inline Value * HashTable<Key, Value, HashingFunction>::Search(const Key & key)
{
	KeyValueListNode * kvln = this->Find(key);
	if (kvln)
	{
		return &kvln->value;
	}
	return nullptr;
}

template<typename Key, typename Value, size_t(*HashingFunction)(const Key &)>
inline void HashTable<Key, Value, HashingFunction>::Print(std::ostream & out) const
{
	for (HashTable::Iterator it = this->Begin(); !it->Done(); ++it)
	{
		out << (*it)->key << ' ' << (*it)->value << '\n';
	}
}
