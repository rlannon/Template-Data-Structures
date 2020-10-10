/*

linked_list.h
An implementation of a singly-linked-list using C++ templates
The list also includes a forward iterator. std::iterator_traits are defined for this iterator using typedefs

*/

#pragma once

#include <exception>
#include <iostream>
#include <iterator>
#include <initializer_list>
#include <cstddef>	// for ptrdiff_t
#include <type_traits>

#include "node.h"

template < typename T, typename Allocator = std::allocator< list_node<T> > >
class linked_list
{
	Allocator _allocator;

	list_node<T>* head;
	list_node<T>* tail;

	size_t length;

	void _push_back(list_node<T> to_append);
public:
	typedef T value_type;
	typedef Allocator allocator_type;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T* pointer;
	typedef const T* const_pointer;

	// define the iterator for the linked list
	template<bool is_const = false>
	class list_iterator
	{
		list_node<value_type>* ptr;
		list_iterator(list_node<value_type>* ptr);
	public:
		// define _all_ of std::iterator_traits struct members or it won't work
		using value_type = typename std::conditional<is_const, const T, T>::type;
		using iterator_category = std::forward_iterator_tag;
		using difference_type = ptrdiff_t;
		using pointer = value_type*;
		using reference = value_type&;
		using const_pointer = const T *;
		using const_reference = const value_type &;

		bool operator==(const list_iterator right)
		{
			return this->ptr == right.ptr;
		}

		bool operator!=(const list_iterator right)
		{
			return this->ptr != right.ptr;
		}

		reference operator*()
		{
			return *this->ptr;
		}

		pointer operator->()
		{
			return this->ptr;
		}

		list_iterator& operator++()
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

		list_iterator operator++(int)
		{
			/*

			Postfix operator (e.g., iterator++)
			This will fetch the value at the iterator _before_ incrementing it and return the old value

			*/

			if (this->ptr)
			{
				iterator to_return(*this);
				this->ptr = this->ptr->get_next();
				return to_return;
			}
			else
			{
				throw std::out_of_range("Cannot advance iterator");
			}
		}

		template <bool _is_const = is_const,
			typename std::enable_if<_is_const, int>::value = 1>
		list_iterator& operator=(const list_iterator<false>& right)
		{
			// allows assignment of iterator to const_iterator
			this->ptr = right.ptr;
		}

		template <bool _is_const = is_const,
			typename std::enable_if<_is_const, int>::value = 1>
		list_iterator& operator=(const list_iterator<false>&& right)
		{
			// same as above, except it is move assignment
			this->ptr = right.ptr;
			right = nullptr;
		}

		list_iterator& operator=(const list_iterator& right)
		{
			this->ptr = right.ptr;
			return *this;
		}

		list_iterator& operator=(const list_iterator&& right)
		{
			this->ptr = right.ptr;
			right = nullptr;
		}

		template <bool _is_const = is_const,
			typename std::enable_if<_is_const, int>::value = 1>
		list_iterator(const list_iterator<false>& other)
			: ptr(other.ptr) {}

		template <bool _is_const = is_const,
			typename std::enable_if<_is_const, int>::value = 1>
		list_iterator(const list_iterator<false>&& other)
			: ptr(other.ptr)
		{
			other = nullptr;
		}

		list_iterator(const list_iterator& it): ptr(it.ptr)
		{
			// copy constructor
		}

		list_iterator(const list_iterator&& it) : ptr(it.ptr)
		{
			// move constructor
			it = nullptr;
		}

		list_iterator(): ptr(nullptr)
		{
		}
		
		~list_iterator()
		{
		}
	};

	typedef list_iterator< false > iterator;
	typedef list_iterator< true > const_iterator;

	// iterators
	iterator begin();
	iterator end();
	iterator back();

	const_iterator cbegin() const;
	const_iterator cend() const;

	// add a value to the list
	void push_back(T val);
	void insert(T val, size_t pos);	// insert a value at a given index position
	void insert(T val, iterator pos);	// insert value at a given iterator position
	
	// find a particular value in the list
	list_node<T>* search(T to_find) const;

	// erase a value from the list
	void erase(T val);

	// get the number of elements in the vector
	size_t size() const;

	linked_list(std::initializer_list<T> il);
	linked_list();
	~linked_list();
};

template <typename T, typename Allocator>
typename linked_list<T, Allocator>::iterator linked_list<T, Allocator>::begin()
{
	return iterator(this->head);
}

template <typename T, typename Allocator>
typename linked_list<T, Allocator>::iterator linked_list<T, Allocator>::end()
{
	return iterator(nullptr);
}

template <typename T, typename Allocator>
typename linked_list<T, Allocator>::iterator linked_list<T, Allocator>::back()
{
	return iterator(this->tail);
}

template <typename T, typename Allocator>
typename linked_list<T, Allocator>::const_iterator linked_list<T, Allocator>::cbegin() const
{
	return const_iterator(this->tail);
}

template <typename T, typename Allocator>
typename linked_list<T, Allocator>::const_iterator linked_list<T, Allocator>::cend() const
{
	return const_iterator(nullptr);
}

template<typename T, typename Allocator>
inline void linked_list<T, Allocator>::_push_back(list_node<T> to_append)
{
	// allocate our node
	list_node<T>* temp = std::allocator_traits<Allocator>::allocate(this->_allocator, 1);
	std::allocator_traits<Allocator>::construct(this->_allocator, temp, to_append);

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

template<typename T, typename Allocator>
inline void linked_list<T, Allocator>::push_back(T val)
{
	this->_push_back(list_node<T>(val));		// just use our other append function by creating a node from the value
}

template<typename T, typename Allocator>
inline void linked_list<T, Allocator>::insert(T val, size_t pos)
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
	list_node<T>* previous_node = this->head;
	list_node<T>* current_node = this->head;

	while (current_node != nullptr && current_pos < pos)
	{
		previous_node = current_node;
		current_node = current_node->get_next();
		current_pos += 1;
	}

	// if the current node is nullptr, it means we have gone past the end of the list and should append
	if (current_node == nullptr)
	{
		this->_push_back(val);
	}
	// else, if we are within the list
	else
	{
		list_node<T>* new_node = std::allocator_traits<Allocator>::allocate(this->_allocator, 1);
		std::allocator_traits<Allocator>::construct(this->_allocator, new_node, val);

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

template<typename T, typename Allocator>
inline void linked_list<T, Allocator>::insert(T val, iterator pos)
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
		this->_push_back(val);
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
			list_node<T>* new_node = std::allocator_traits<Allocator>::allocate(this->_allocator, 1);
			std::allocator_traits<Allocator>::construct(this->_allocator, new_node, val);

			list_node<T>* previous_node = previous_pos.get_ptr();

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

template <typename T, typename Allocator>
inline list_node<T>* linked_list<T, Allocator>::search(T to_find) const
{
	// Finds a node in the list by value. Returns the first one.

	linked_list<T, Allocator>::iterator it = this->begin();
	while (it != this->end() && *it != to_find)
	{
		it++;
	}

	// return the value at the iterator; if not found, it will return LinkedList<T>.end()
	return it.get_ptr();
}

template <typename T, typename Allocator>
inline void linked_list<T, Allocator>::erase(T val)
{
	// Erase a value from the list; if the value is not found, throws an exception
	linked_list<T, Allocator>::iterator to_delete = this->begin();
	list_node<T>* prev = to_delete.get_ptr();
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
		std::allocator_traits<Allocator>::destroy(this->_allocator, to_delete.get_ptr());
		std::allocator_traits<Allocator>::deallocate(this->_allocator, to_delete.get_ptr(), 1);

		// decrease the list length by 1
		this->length -= 1;
	}
}

template <typename T, typename Allocator>
inline size_t linked_list<T, Allocator>::size() const
{
	return this->length;
}

template<typename T, typename Allocator>
inline linked_list<T, Allocator>::linked_list(std::initializer_list<T> il)
{
	// allow linked lists to be initialized with std::initializer_list<T>

	if (il.size() > 0)
	{
		this->length = il.size();
		typename std::initializer_list<T>::iterator it = il.begin();
		this->head = std::allocator_traits<Allocator>::allocate(this->_allocator, 1);
		std::allocator_traits<Allocator>::construct(this->_allocator, this->head, *it);

		list_node<T>* current = this->head;
		it++;

		for (; it != il.end(); it++)
		{
			list_node<T>* next = std::allocator_traits<Allocator>::allocate(this->_allocator, 1);
			std::allocator_traits<Allocator>::construct(this->_allocator, next, *it);

			current->set_next(next);
			current = next;
		}

		this->tail = current;
	}
	else
	{
		linked_list();
	}
}

template <typename T, typename Allocator>
inline linked_list<T, Allocator>::linked_list()
{
	// initialize an empty linked list

	this->head = nullptr;
	this->tail = nullptr;
	this->length = 0;
	this->_allocator = Allocator();
}

template<typename T, typename Allocator>
inline linked_list<T, Allocator>::~linked_list()
{
	if (this->length > 0) {
		// if our head isn't a nullptr, we have to iterate through the list and delete all of our nodes to avoid a memory leak
		if (this->head != nullptr)
		{
			list_node<T>* current = this->head;
			while (current)
			{
				// update the current node to the next node in the list
				list_node<T>* temp = current;
				current = current->get_next();

				// delete the dynamic object and set the pointer to 0x00
				std::allocator_traits<Allocator>::destroy(this->_allocator, temp);
				std::allocator_traits<Allocator>::deallocate(this->_allocator, temp, 1);
				temp = nullptr;
			}
		}
	}
	// if the length is > 0, we are done
}
