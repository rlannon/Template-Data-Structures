/*

Algorithms and Data Structures
Copyright 2019 Riley Lannon
queue.h

An implementation of the Queue data structures using C++ templates and STL Allocators

*/

#pragma once

#include <initializer_list>
#include <memory>
#include <stdexcept>

template <typename T, typename Allocator = std::allocator<T>>
class queue
{
	Allocator queue_allocator;

	size_t _size;
	size_t _capacity;

	T* buffer;	// buffer total
public:
	size_t size() const;
	size_t capacity() const;
	bool empty() const;

	void push_back(T to_push);
	T peek_front();
	T pop_front();

	explicit queue(std::initializer_list<T> il);
	explicit queue();
	~queue();
};

/*

Getters

*/

template<typename T, typename Allocator>
inline size_t queue<T, Allocator>::size() const
{
	return this->_size();
}

template<typename T, typename Allocator>
inline size_t queue<T, Allocator>::capacity() const
{
	return this->_capacity;
}

template<typename T, typename Allocator>
inline bool queue<T, Allocator>::empty() const
{
	return this->_size == 0;
}

template<typename T, typename Allocator>
inline void queue<T, Allocator>::push_back(T to_push)
{
	if (this->_capacity == 0)
	{
		this->buffer = this->queue_allocator.allocate(1);
		this->_capacity = 1;
	}
	else if (this->_size == this->_capacity)
	{
		size_t new_capacity = this->_capacity * 1.5;
		if (new_capacity < 4)
		{
			new_capacity *= 2;
		}

		// on a resize, we need to copy the contents
		T* old_buf = this->buffer;
		this->buffer = this->queue_allocator.allocate(new_capacity);

		if (this->buffer)
		{
			memcpy(buffer, old_buf, this->_size * sizeof(T));
			this->queue_allocator.deallocate(old_buf, this->_size);

			this->_capacity = new_capacity;
		}
		else
		{
			throw std::runtime_error("Could not allocate memory");
		}
	}

	// finally, perform the push
	std::allocator_traits<Allocator>::construct(this->queue_allocator, &this->buffer[this->_size], to_push);
	this->_size += 1;

	return;
}

template<typename T, typename Allocator>
inline T queue<T, Allocator>::peek_front()
{
	if (this->_size != 0)
	{
		return this->buffer[this->_size - 1];
	}
	else
	{
		throw std::out_of_range("Cannot peek on empty queue");
	}
}

template<typename T, typename Allocator>
inline T queue<T, Allocator>::pop_front()
{
	T to_return;

	if (this->_size > 0)
	{
		// first, fetch the element
		to_return = this->buffer[0];

		// destroy the element and update the size
		std::allocator_traits<Allocator>::destroy(this->queue_allocator, this->buffer);
		this->_size -= 1;

		// now, move all of the elements in the queue over
		for (size_t i = 0; i < this->_size; i++)
		{
			this->buffer[i] = this->buffer[i + 1];
		}
	}
	else
	{
		throw std::out_of_range("Cannot pop from empty queue");
	}

	return to_return;
}

/*

Constructors and Destructor

*/

template <typename T, typename Allocator>
inline queue<T, Allocator>::queue(std::initializer_list<T> il)
{
	/*

	Allow queues to be initialized with initializer-lists
	Note that elements are pushed _in order_ from left to right

	*/

	// construct our allocator
	this->queue_allocator = Allocator();
	
	// allocate space for our queue
	this->_capacity = ((il.size() * 1.5) < 4) ? (il.size() * 2) : (il.size() * 1.5);
	this->buffer = this->queue_allocator.allocate(this->_capacity);
	this->_size = 0;

	// push every element in the list
	for (T elem: il)
	{
		std::allocator_traits<Allocator>::construct(this->queue_allocator, &this->buffer[this->_size], elem);
		this->_size += 1;
	}
}

template<typename T, typename Allocator>
inline queue<T, Allocator>::queue()
{
	this->buffer = nullptr;
	this->_capacity = 0;
	this->_size = 0;
	this->queue_allocator = Allocator();
}

template<typename T, typename Allocator>
inline queue<T, Allocator>::~queue()
{
	if (this->_size != 0)
	{
		for (size_t i = 0; i < this->_size; i++)
		{
			std::allocator_traits<Allocator>::destroy(this->queue_allocator, &this->buffer[i]);
			this->_size -= 1;
		}
	}

	this->queue_allocator.deallocate(this->buffer, this->_capacity);
	this->_capacity = 0;
	this->buffer = nullptr;
}

