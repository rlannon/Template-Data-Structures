/*

queue.h
An implementation of a queue using C++ templates and STL allocators

*/

#pragma once

#include <initializer_list>
#include <memory>
#include <stdexcept>

// todo: reserve empty space on the front of the queue so we don't have to move elements as often?

template <typename T, typename Allocator = std::allocator<T>>
class queue
{
	Allocator _queue_allocator;

	size_t _size;
	size_t _capacity;

	T* _buffer;	// buffer total
public:
	size_t max_size() const;
	size_t size() const;
	size_t capacity() const;
	bool empty() const;

	void push_back(T to_push);
	T& peek_front();
	T pop_front();

	explicit queue(std::initializer_list<T> il);
	explicit queue();
	~queue();
};

/*

Getters

*/

template<typename T, typename Allocator>
inline size_t queue<T, Allocator>::max_size() const
{
	return std::allocator_traits<Allocator>::max_size(this->_queue_allocator);
}

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
		this->_capacity = 4;
		this->_buffer = std::allocator_traits<Allocator>::allocate(this->_queue_allocator, this->_capacity);
	}
	else if (this->_size == this->_capacity)
	{
		size_t new_capacity = this->_capacity * 1.5;
		if (new_capacity < 4)
		{
			new_capacity *= 2;
		}

		// on a resize, we need to copy the contents
		T* old_buf = this->_buffer;
		this->_buffer = std::allocator_traits<Allocator>::allocate(this->_queue_allocator, new_capacity);

		if (this->_buffer)
		{
			// construct new elements and delete the old ones
			for (size_t i = 0; i < this->_size; i++) {
				T *to_construct = &old_buf[i];
				std::allocator_traits<Allocator>::construct(this->_queue_allocator, &this->_buffer[i], *to_construct);
				std::allocator_traits<Allocator>::destroy(this->_queue_allocator, to_construct);
			}

			// deallocate the old buffer
			std::allocator_traits<Allocator>::deallocate(this->_queue_allocator, old_buf, this->_capacity);
			this->_capacity = new_capacity;
		}
		else
		{
			throw std::runtime_error("Could not allocate memory");
		}
	}

	// finally, perform the push utilizing placement new
	T *addr = &this->_buffer[this->_size];
	std::allocator_traits<Allocator>::construct(this->_queue_allocator, addr, to_push);
	this->_size += 1;

	return;
}

template<typename T, typename Allocator>
inline T& queue<T, Allocator>::peek_front()
{
	if (this->_size != 0)
	{
		return this->_buffer[this->_size - 1];
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
		to_return = this->_buffer[0];

		// destroy the element using placement delete
		T *to_delete = &this->_buffer[this->_size - 1];
		std::allocator_traits<Allocator>::destroy(this->_queue_allocator, to_delete);

		// now, move all of the elements in the queue over
		for (size_t i = 0; i < this->_size; i++)
		{
			this->_buffer[i] = this->_buffer[i + 1];
		}

		// update the size
		this->_size -= 1;
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
	this->_queue_allocator = Allocator();
	
	// allocate space for our queue
	this->_capacity = ((il.size() * 1.5) < 4) ? (il.size() * 2) : (il.size() * 1.5);
	this->_buffer = std::allocator_traits<Allocator>::allocate(this->_queue_allocator, this->_capacity);
	this->_size = 0;

	// push every element in the list
	for (T elem: il)
	{
		T *addr = &this->_buffer[this->_size];
		std::allocator_traits<Allocator>::construct(this->_queue_allocator, addr, elem);
		this->_size += 1;
	}
}

template<typename T, typename Allocator>
inline queue<T, Allocator>::queue()
{
	this->_buffer = nullptr;
	this->_capacity = 0;
	this->_size = 0;
	this->_queue_allocator = Allocator();
}

template<typename T, typename Allocator>
inline queue<T, Allocator>::~queue()
{
	for (size_t i = 0; i < this->_size; i++)
	{
		T *to_destroy = &this->_buffer[i];
		std::allocator_traits<Allocator>::destroy(this->_queue_allocator, to_destroy);
	}
	this->_size = 0;

	std::allocator_traits<Allocator>::deallocate(this->_queue_allocator, this->_buffer, this->_capacity);
	this->_capacity = 0;
	this->_buffer = nullptr;
}

