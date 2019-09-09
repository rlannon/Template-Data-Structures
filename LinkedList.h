/*

Algorithms and Data Structures
Copyright 2019 Riley Lannon
LinkedList.h

An implementation of a singly-linked-list using C++ templates

*/

#pragma once

#include <exception>
#include <iostream>
#include "Node.h"
#include "ListIterator.h"

template <class T>
class LinkedList
{
	Node<T>* head;
	Node<T>* tail;

	size_t length;

	void append(Node<T> node);
public:
	typedef list_iterator<T> iterator;

	// iterators
	list_iterator<T> begin() const;
	list_iterator<T> back() const;
	list_iterator<T> end() const;

	// add a value to the list
	void append(T val);
	void insert(T val, size_t pos);	// insert a value at a given index position
	void insert(T val, list_iterator<T> pos);	// insert value at a given iterator position
	
	// find a particular value in the list
	Node<T>* search(T to_find) const;

	// erase a value from the list
	void erase(T val);

	// get the length
	size_t get_length() const;

	LinkedList();
	~LinkedList();
};

template <class T>
list_iterator<T> LinkedList<T>::begin() const
{
	return list_iterator<T>(this->head);
}

template <class T>
list_iterator<T> LinkedList<T>::back() const
{
	return list_iterator<T>(this->tail);
}

template <class T>
list_iterator<T> LinkedList<T>::end() const
{
	return list_iterator<T>(nullptr);
}

template<class T>
inline void LinkedList<T>::append(Node<T> node)
{
	Node<T>* temp = new Node<T>(node);	// allocate the new node

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
inline void LinkedList<T>::append(T val)
{
	this->append(Node<T>(val));		// just use our other append function by creating a node from the value
}

template<class T>
inline void LinkedList<T>::insert(T val, size_t pos)
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
	Node<T>* previous_node = this->head;
	Node<T>* current_node = this->head;

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
		Node<T>* new_node = new Node<T>(val);

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
inline void LinkedList<T>::insert(T val, list_iterator<T> pos)
{
	list_iterator<T> current_pos = this->begin();
	list_iterator<T> previous_pos = this->begin();

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
			Node<T>* new_node = new Node<T>(val);
			Node<T>* previous_node = previous_pos.get_ptr();

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

template <class T>
inline Node<T>* LinkedList<T>::search(T to_find) const
{
	// Finds a node in the list by value. Returns the first one.

	LinkedList<T>::iterator it = this->begin();
	while (it != this->end() && *it != to_find)
	{
		it++;
	}

	// return the value at the iterator; if not found, it will return LinkedList<T>.end()
	return it.get_ptr();
}

template <class T>
inline void LinkedList<T>::erase(T val)
{
	// Erase a value from the list; if the value is not found, throws an exception
	LinkedList<T>::iterator to_delete = this->begin();
	Node<T>* prev = to_delete.get_ptr();
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
		// prev should now point to the Node that comes after to_delete
		prev->set_next(to_delete.get_ptr()->get_next());

		// delete the node
		delete to_delete.get_ptr();

		// decrease the list length by 1
		this->length -= 1;
	}
}

template <class T>
inline size_t LinkedList<T>::get_length() const
{
	return this->length;
}

template <class T>
inline LinkedList<T>::LinkedList()
{
	this->head = nullptr;
	this->tail = nullptr;
	this->length = 0;
}

template<class T>
inline LinkedList<T>::~LinkedList()
{
	if (this->length > 0) {
		// if our head isn't a nullptr, we have to iterate through the list and delete all of our nodes to avoid a memory leak
		if (this->head != nullptr)
		{
			Node<T>* current = this->head;
			while (current)
			{
				// update the current node to the next node in the list
				Node<T>* temp = current;
				current = current->get_next();

				// delete the dynamic object and set the pointer to 0x00
				delete temp;
				temp = nullptr;
			}
		}
		// otherwise, we have a serious error
		else
		{
			throw std::runtime_error("Expected list elements but found a head value of nullptr");
		}
	}
	// if the length is > 0, we are done
}


