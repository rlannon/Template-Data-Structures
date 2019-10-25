/*

Algorithms and Data Structures
Copyright 2019 Riley Lannon
LinkedList.h

An implementation of a singly-linked-list using C++ templates
The list also includes a forward iterator. std::iterator_traits are defined for this iterator using typedefs

*/

#pragma once

#include <exception>
#include <iostream>
#include <iterator>
#include <cstddef>	// for ptrdiff_t

#include "node.h"

template <typename T>
class linked_list
{
	node<T>* head;
	node<T>* tail;

	size_t length;

	void append(node<T> to_append);
public:
	typedef T value_type;	// must define a value type for the container

	// define the iterator for the linked list
	class iterator
	{
		node<T>* ptr;
	public:
		// define _all_ of std::iterator_traits struct members or it won't work
		typedef node<T> value_type;
		typedef std::forward_iterator_tag iterator_category;
		typedef ptrdiff_t difference_type;
		typedef node<T>* pointer;
		typedef node<T>& reference;

		bool operator==(const iterator right);
		bool operator!=(const iterator right);
		node<T>& operator*();
		node<T>* operator->() const;
		iterator& operator++();
		iterator operator++(int);

		iterator(node<T>* ptr);
		iterator();
		~iterator();
	};

	// iterators
	iterator begin() const;
	iterator back() const;
	iterator end() const;

	// add a value to the list
	void append(T val);
	void insert(T val, size_t pos);	// insert a value at a given index position
	void insert(T val, iterator pos);	// insert value at a given iterator position
	
	// find a particular value in the list
	node<T>* search(T to_find) const;

	// erase a value from the list
	void erase(T val);

	// get the length
	size_t get_length() const;

	linked_list();
	~linked_list();
};

template <typename T>
typename linked_list<T>::iterator linked_list<T>::begin() const
{
	return iterator(this->head);
}

template <typename T>
typename linked_list<T>::iterator linked_list<T>::back() const
{
	return iterator(this->tail);
}

template <typename T>
typename linked_list<T>::iterator linked_list<T>::end() const
{
	return iterator(nullptr);
}

template<class T>
inline void linked_list<T>::append(node<T> to_append)
{
	node<T>* temp = new node<T>(to_append);	// allocate the new node

	// if we don't have anything in the list, set "head" to point to the node
	if (this->head == nullptr)
	{
		this->head = temp;
	}
	else {
		this->tail->set_next(temp);	// set the current tail's "next" member to point to our new node
	}

	this->length += 1;

	// finally, set the list's "next" member to be the new node
	this->tail = temp;
}

template<class T>
inline void linked_list<T>::append(T val)
{
	this->append(node<T>(val));		// just use our other append function by creating a node from the value
}

template<class T>
inline void linked_list<T>::insert(T val, size_t pos)
{
	/*
	
	Inserts a node to be the /new node at that position/; so, if we have the following LinkedList<int>:
		0	1	2	3	4
	And we call insert(7, 2)
	The value will be in the new position 2, resulting in the rest being pushed over, like so:
		0	1	7	2	3	4
	If we call insert(7, < number >= list length>), we will append
	
	*/
	
	size_t current_pos = 0;
	node<T>* previous_node = this->head;
	node<T>* current_node = this->head;

	while (current_node != nullptr && current_pos < pos)
	{
		previous_node = current_node;
		current_node = current_node->get_next();
		current_pos += 1;
	}

	// if the current node is nullptr, it means we have gone past the end of the list and should append
	if (current_node == nullptr)
	{
		this->append(val);
	}
	// else, if we are within the list
	else
	{
		node<T>* new_node = new node<T>(val);

		// if we are inserting at position 0, we need to update LinkedList<T>::head
		if (current_pos == 0)
		{
			new_node->set_next(this->head);
			this->head = new_node;
		}
		else {
			new_node->set_next(current_node);
			previous_node->set_next(new_node);
		}
		this->length += 1;
	}
}

template<class T>
inline void linked_list<T>::insert(T val, iterator pos)
{
	iterator current_pos = this->begin();
	iterator previous_pos = this->begin();

	// if the iterator is at begin() or end(), it is an easy operation
	if (pos == this->begin())
	{
		this->insert(val, 0);
	}
	else if (pos == this->end())
	{
		this->append(val);
	}
	// otherwise, iterate
	else {
		while (current_pos != this->end() && current_pos != pos)
		{
			previous_pos = current_pos;
			current_pos++;
		}

		// safety check
		if (current_pos != this->end())
		{
			node<T>* new_node = new node<T>(val);
			node<T>* previous_node = previous_pos.get_ptr();

			// current_pos is where we want new_node to go; set prev -> new -> cur
			previous_node->set_next(new_node);
			new_node->set_next(current_pos.get_ptr());

			this->length += 1;
		}
		else {
			throw std::out_of_range("Iterator out of range");
		}
	}
}

template <typename T>
inline node<T>* linked_list<T>::search(T to_find) const
{
	// Finds a node in the list by value. Returns the first one.

	linked_list<T>::iterator it = this->begin();
	while (it != this->end() && *it != to_find)
	{
		it++;
	}

	// return the value at the iterator; if not found, it will return LinkedList<T>.end()
	return it.get_ptr();
}

template <typename T>
inline void linked_list<T>::erase(T val)
{
	// Erase a value from the list; if the value is not found, throws an exception
	linked_list<T>::iterator to_delete = this->begin();
	node<T>* prev = to_delete.get_ptr();
	bool done = false;

	while (to_delete != this->end() && !done)
	{
		to_delete++;
		if (to_delete.get_ptr()->get_data() == val)
		{
			done = true;
		}
		else
		{
			prev = to_delete.get_ptr();
		}
	}

	// if to_delete is the past-the-end iterator, we couldn't find val in the list
	if (to_delete == this->end())
	{
		throw std::runtime_error("Could not find value in list!");
	}
	else
	{
		// prev should now point to the node that comes after to_delete
		prev->set_next(to_delete.get_ptr()->get_next());

		// delete the node
		delete to_delete.get_ptr();

		// decrease the list length by 1
		this->length -= 1;
	}
}

template <typename T>
inline size_t linked_list<T>::get_length() const
{
	return this->length;
}

template <typename T>
inline linked_list<T>::linked_list()
{
	this->head = nullptr;
	this->tail = nullptr;
	this->length = 0;
}

template<class T>
inline linked_list<T>::~linked_list()
{
	if (this->length > 0) {
		// if our head isn't a nullptr, we have to iterate through the list and delete all of our nodes to avoid a memory leak
		if (this->head != nullptr)
		{
			node<T>* current = this->head;
			while (current)
			{
				// update the current node to the next node in the list
				node<T>* temp = current;
				current = current->get_next();

				// delete the dynamic object and set the pointer to 0x00
				delete temp;
				temp = nullptr;
			}
		}
	}
	// if the length is > 0, we are done
}

/*

iterator functions

*/

template <typename T>
bool linked_list<T>::iterator::operator==(const linked_list<T>::iterator right)
{
	return this->ptr == right.ptr;
}

template <typename T>
bool linked_list<T>::iterator::operator!=(const linked_list<T>::iterator right)
{
	return this->ptr != right.ptr;
}

template <typename T>
node<T>& linked_list<T>::iterator::operator*()
{
	return *this->ptr;
}

template <typename T>
node<T>* linked_list<T>::iterator::operator->() const
{
	return this->ptr;
}

template <typename T>
typename linked_list<T>::iterator& linked_list<T>::iterator::operator++()
{
	/*
	
	Prefix operator (++iterator)
	This will increment the value at the iterator and then return it
	
	*/

	if (this->ptr)
	{
		this->ptr = this->ptr->get_next();
		return *this;
	}
	else
	{
		throw std::out_of_range("Cannot advance iterator");
	}
}

template <typename T>
typename linked_list<T>::iterator linked_list<T>::iterator::operator++(int)
{
	/*
	
	Postfix operator (e.g., iterator++)
	This will fetch the value at the iterator _before_ incrementing it and return the old value
	
	*/
	
	if (this->ptr)
	{
		iterator &to_return(*this);
		this->ptr = this->ptr->get_next();
		return to_return;
	}
	else
	{
		throw std::out_of_range("Cannot advance iterator");
	}
}

template <typename T>
linked_list<T>::iterator::iterator(node<T>* ptr)
{
	this->ptr = ptr;
}

template <typename T>
linked_list<T>::iterator::iterator()
{
	this->ptr = nullptr;
}

template <typename T>
linked_list<T>::iterator::~iterator()
{
	
}
