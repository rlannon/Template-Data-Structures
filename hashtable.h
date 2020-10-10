/*

Algorithms and Data Structures
Copyright 2019 Riley Lannon
hashtable.h

An implementation of a hash table in C++ using templates.

*/

#pragma once

#include <memory>
#include <iostream>
#include <stdexcept>

#include "linked_list.h"
#include "default_hash.h"

template <typename K, typename V, typename Hash = default_hash<K>, typename Allocator = std::allocator<K>>
class hash_table
{
	struct entry;

	Allocator table_allocator;
	
	size_t _size;	// the number of entries
	size_t _capacity;	// the number of possible entries
	linked_list<entry> *buckets;	// the buffer is an array of linked lists

	Hash hash_function;	// the class that will provide the hash function
public:
	// we will use separate chaining here, so we need an 'entry' struct to store the key with the data
	struct entry
	{
		K key;
		V data;

		entry(K key, V data)
		{
			this->key = key;
			this->data = data;
		}
		entry()
		{

		}
	};

	// Define the iterator for our hash table
	class iterator
	{
		list_node<entry>* ptr;
	public:
		typedef entry value_type;
		typedef std::forward_iterator_tag iterator_category;
		typedef ptrdiff_t difference_type;
		typedef entry* pointer;
		typedef entry& reference;

		bool operator==(const iterator right);
		bool operator!=(const iterator right);
		list_node<entry>& operator*();
		list_node<entry>* operator->();
		iterator& operator++();
		iterator operator++(int);

		iterator(list_node<entry>& ptr);
		//iterator(typename linked_list<entry>::iterator it);
		iterator();
		~iterator();
	};

	// define iterator functions
	iterator begin() const;
	iterator end() const;

	typedef K key_type;
	typedef V mapped_type;

	// operators
	mapped_type& operator[](const key_type& right);

	size_t size() const;
	size_t capacity() const;
	bool empty() const;

	mapped_type& at(const key_type& key);
	entry& insert(K key, V value);
	iterator find(K to_find);

	hash_table(size_t capacity = 16);
	~hash_table();
};


/*

ITERATOR FUNCTIONS

*/

// operators

template <typename K, typename V, typename Hash, typename Allocator>
bool hash_table<K, V, Hash, Allocator>::iterator::operator==(const typename hash_table<K, V, Hash, Allocator>::iterator right)
{
	return this->ptr == right.ptr;
}

template <typename K, typename V, typename Hash, typename Allocator>
bool hash_table<K, V, Hash, Allocator>::iterator::operator!=(const typename hash_table<K, V, Hash, Allocator>::iterator right)
{
	return this->ptr != right.ptr;
}

template <typename K, typename V, typename Hash, typename Allocator>
typename hash_table<K, V, Hash, Allocator>::iterator& hash_table<K, V, Hash, Allocator>::iterator::operator++()
{
	// prefix ++ operator
	// increment iterator to next element in the bucket, else return hash_table::end
	if (this->ptr)
	{
		this->ptr = this->ptr->next;
		return *this;
	}
	else
	{
		throw std::out_of_range("Cannot advance iterator");
	}
}

template <typename K, typename V, typename Hash, typename Allocator>
typename hash_table<K, V, Hash, Allocator>::iterator hash_table<K, V, Hash, Allocator>::iterator::operator++(int)
{
	// prefix ++ operator
	// increment iterator to next element in the bucket, else return hash_table::end
	if (this->ptr)
	{
		iterator to_return(*this);
		to_return = this->ptr->next;
		return to_return;
	}
	else
	{
		throw std::out_of_range("Cannot advance iterator");
	}
}

template <typename K, typename V, typename Hash, typename Allocator>
list_node<typename hash_table<K, V, Hash, Allocator>::entry>& hash_table<K, V, Hash, Allocator>::iterator::operator*()
{
	return *this->ptr;
}

template <typename K, typename V, typename Hash, typename Allocator>
list_node<typename hash_table<K, V, Hash, Allocator>::entry>* hash_table<K, V, Hash, Allocator>::iterator::operator->()
{
	return this->ptr;
}

// Constructors, destructor

template <typename K, typename V, typename Hash, typename Allocator>
hash_table<K, V, Hash, Allocator>::iterator::iterator(list_node<entry>& ptr)
{
	this->ptr = &ptr;
}

template <typename K, typename V, typename Hash, typename Allocator>
hash_table<K, V, Hash, Allocator>::iterator::iterator()
{
	this->ptr = nullptr;
}

template <typename K, typename V, typename Hash, typename Allocator>
hash_table<K, V, Hash, Allocator>::iterator::~iterator()
{
}


/*

HASH TABLE FUNCTIONS

*/

// Hash function iterator returns

template <typename K, typename V, typename Hash, typename Allocator>
typename hash_table<K, V, Hash, Allocator>::iterator hash_table<K, V, Hash, Allocator>::begin() const
{
	typename linked_list<entry>::iterator first_entry = this->buckets[0].begin();
	list_node<entry> &first = *first_entry;
	return iterator(first);
}

template <typename K, typename V, typename Hash, typename Allocator>
typename hash_table<K, V, Hash, Allocator>::iterator hash_table<K, V, Hash, Allocator>::end() const
{
	return iterator();
}

// Operators

template <typename K, typename V, typename Hash, typename Allocator>
typename hash_table<K, V, Hash, Allocator>::mapped_type& hash_table<K, V, Hash, Allocator>::operator[](const typename hash_table<K, V, Hash, Allocator>::key_type& right)
{
	// if we can find the element, return a reference
	hash_table<K, V, Hash, Allocator>::iterator it = this->find(right);
	if (it != this->end())
	{
		mapped_type& value = it->get_data().data;
		return value;
	}
	// otherwise, insert it and return a reference to the data member
	else
	{
		this->_size += 1;
		entry& new_entry = this->insert(right, V());
		return new_entry.data;
	}
}

// Size, capcity, empty

template <typename K, typename V, typename Hash, typename Allocator>
size_t hash_table<K, V, Hash, Allocator>::size() const
{
	return this->_size;
}

template <typename K, typename V, typename Hash, typename Allocator>
size_t hash_table<K, V, Hash, Allocator>::capacity() const
{
	return this->_capacity;
}

template <typename K, typename V, typename Hash, typename Allocator>
bool hash_table<K, V, Hash, Allocator>::empty() const
{
	return this->_size == 0;
}

// Accesses

template<typename K, typename V, typename Hash, typename Allocator>
typename hash_table<K, V, Hash, Allocator>::mapped_type& hash_table<K, V, Hash, Allocator>::at(const hash_table<K, V, Hash, Allocator>::key_type& key)
{
	// returns a reference to the mapped type if the key is found; if it is not found, throws an out_of_range exception
	iterator it = this->find(key);
	if (it == this->end())
	{
		throw std::out_of_range("Could not find the specified key in the hash table");
	}
	else
	{
		entry& found = it->get_data();
		return found.data;
	}
}

template<typename K, typename V, typename Hash, typename Allocator>
typename hash_table<K, V, Hash, Allocator>::entry& hash_table<K, V, Hash, Allocator>::insert(K key, V value)
{
	// adds the specified key-value pair to the table

	// the index is the hash value modulo the capacity of the table
	size_t index = this->hash_function(key) % this->_capacity;

	// check to see if this key appears in the list
	typename linked_list<entry>::iterator it = this->buckets[index].begin();
	bool found = false;
	while (it != this->buckets[index].end() && !found)
	{
		if (it->get_data().key == key)
		{
			found = true;
		}
		else
		{
			it++;
		}
	}

	if (found)
	{
		// todo: find better exception for duplicate key
		throw std::runtime_error("Duplicate key");

		// return the value at the duplicate key on exception
		list_node<entry>& to_return = *it;
		return to_return.get_data();
	}
	else
	{
		// append this to the linked list at that index
		this->buckets[index].append(entry(key, value));
		this->_size += 1;	// we have one more entry in the table

		// return a reference to the new entry
		list_node<entry>& to_return = *this->buckets[index].back();
		return to_return.get_data();
	}
}

template <typename K, typename V, typename Hash, typename Allocator>
typename hash_table<K, V, Hash, Allocator>::iterator hash_table<K, V, Hash, Allocator>::find(K to_find)
{
	// get the index the same way we did in the insert function
	size_t index = this->hash_function(to_find) % this->_capacity;

	// if there is no list at the index, return this->end()
	if (this->buckets[index].begin() == this->buckets[index].end())
	{
		return this->end();
	}
	else
	{
		typename linked_list<entry>::iterator it = this->buckets[index].begin();
		bool found = false;
		while (it != this->buckets[index].end() && !found)
		{
			if (it->get_data().key == to_find)
				found = true;
			else
				it++;
		}

		// return the iterator to the element if found, or a past-the-end iterator if not
		if (found)
			return iterator(*it);
		else
			return iterator();
	}
}

template<typename K, typename V, typename Hash, typename Allocator>
hash_table<K, V, Hash, Allocator>::hash_table(size_t capacity)
{
	// set the capacity; it should round to the nearest power of two but default to 16
	if (capacity < 16)
	{
		this->_capacity = 16;
	}
	else if (capacity > 16)
	{ 
		// round to nearest power of two; works for 32-bit ints
		capacity--;
		capacity |= capacity >> 1;
		capacity |= capacity >> 2;
		capacity |= capacity >> 4;
		capacity |= capacity >> 8;
		capacity |= capacity >> 16;
		capacity++;

		// set our capacity
		this->_capacity = capacity;
	}
	else
	{
		this->_capacity = capacity;	// set the capacity
	}

	this->_size = 0;
	this->hash_function = Hash();	// set our hash function
	this->buckets = new linked_list<entry>[this->_capacity];	// dynamically allocate the buckets as an array of linked lists
}

template<typename K, typename V, typename Hash, typename Allocator>
hash_table<K, V, Hash, Allocator>::~hash_table()
{
	// since we dynamically allocated our buckets, be sure to call delete[]
	delete[] this->buckets;
}
