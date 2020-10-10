#pragma once

/*

deque.h
A custom double-ended queue implementation using templates and STL allocators

*/

#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <type_traits>

// todo: reserve space on both ends so we don't have to move elements every single time we push or pop the front
// this would require us to have some sort of limit on the amount of empty space we can have in the deque
// we could also track how many pushes/pops on each side and utilize a heuristic to aid in making reallocations reflect where we are actually placing the most data
// e.g., if most data is being pushed onto the back, the allocator should end up resizing with more space on the back than the front

template < typename T, typename Allocator = std::allocator<T> >
class deque
{
	Allocator _allocator;

	size_t _size;
	size_t _capacity;
	size_t _first_index;
	size_t _rend_index;
	size_t _end_index;

	T *_buffer;

	size_t _last_index() {
		if (this->empty()) {
			return this->_first_index;
		}
		else {
			return this->_size + this->_first_index - 1;
		}
	}

	void _realloc_expand_buffer() {
		/*
		
		_realloc_expand_buffer
		Automatically reallocates the buffer to be 1.5x its current capacity
		
		*/
		
		// note: ensure some space remains on the front of the new buffer
		size_t new_capacity = (size_t)(this->_capacity * 1.5);
		if (new_capacity < 4) {
			new_capacity *= 2;
		}

		// get the amount of space to put on the front
		size_t new_first = 0;
		if (this->_first_index == 1) {
			size_t size_diff = new_capacity - this->_capacity;
			new_first = size_diff / 2;
		}

		T *old_buffer = this->_buffer;
		this->_buffer = std::allocator_traits<Allocator>::allocate(this->_allocator, new_capacity + 2);
		// construct new elements
		for (size_t i = 0; i < this->_size; i++) {
			T *to_construct = &old_buffer[this->_first_index + i];
			std::allocator_traits<Allocator>::construct(this->_allocator, &this->_buffer[new_first + i], *to_construct);
			std::allocator_traits<Allocator>::destroy(this->_allocator, to_construct);
		}

		// deallocate the old buffer
		std::allocator_traits<Allocator>::deallocate(this->_allocator, old_buffer, this->_capacity + 2);
		this->_capacity = new_capacity;
		this->_first_index = new_first;

		return;
	}

public:
	// some typedefs
	typedef T value_type;
	typedef Allocator allocator_type;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T* pointer;
	typedef const T* const_pointer;

	// capacity

	size_t size() const {
		return this->_size;
	}

	size_t max_size() const {
		return std::allocator_traits<Allocator>::max_size(this->_allocator);
	}

	size_t capacity() const {
		return this->_capacity;
	}

	bool empty() const {
		return this->_size == 0;
	}

	// todo: handle end() and rend() for past-the-end elements
	// this means these elements must exist in the container

	// a random-access iterator
	template <bool is_const = false>
	class deque_iterator
	{
		pointer ptr;

		deque_iterator(pointer p) {
			this->ptr = p;
		}
	public:
		using value_type = typename std::conditional<is_const, const T, T>::type;
		using iterator_category = std::random_access_iterator_tag;
		using difference_type = ptrdiff_t;
		using pointer = value_type*;
		using reference = value_type&;
		using const_pointer = const T*;
		using const_reference = const T&;

		bool operator==(const deque_iterator& right) {
			return this->ptr == right.ptr;
		}

		bool operator!=(const deque_iterator& right) {
			return this->ptr != right.ptr;
		}

		bool operator<(const deque_iterator& right) {
			return this->ptr < right.ptr;
		}

		bool operator>(const deque_iterator& right) {
			return this->ptr > right.ptr;
		}

		bool operator>=(const deque_iterator& right) {
			return this->ptr >= right.ptr;
		}

		bool operator<=(const deque_iterator& right) {
			return this->ptr <= right.ptr;
		}

		difference_type operator+(const difference_type& right) {
			return this->ptr + right;
		}

		difference_type operator-(const difference_type& right) {
			return this->ptr - right;
		}

		difference_type operator-(const deque_iterator& right) {
			return this->ptr - right.ptr;
		}
		
		reference operator*() {
			return *this->ptr;
		}

		pointer operator->() const {
			return this->ptr;
		}
		
		deque_iterator& operator++() {
			if (this->ptr) {
				this->ptr++;
				return *this;
			}
			else {
				throw std::out_of_range("deque");
			}
		}

		deque_iterator operator++(int) {
			if (this->ptr) {
				deque_iterator to_return(*this);
				this->ptr++;
				return to_return;
			}
			else {
				throw std::out_of_range("deque");
			}
		}

		deque_iterator& operator--() {
			if (this->ptr) {
				this->ptr--;
				return *this;
			}
			else {
				throw std::out_of_range("deque");
			}
		}

		deque_iterator operator--(int) {
			if (this->ptr) {
				deque_iterator to_return(*this);
				this->ptr++;
				return to_return;
			}
			else {
				throw std::out_of_range("deque");
			}
		}

		deque_iterator& operator=(const T& right) {
			this->ptr = &right;
			return *this;
		}

		deque_iterator& operator+=(const difference_type& right) {
			this->ptr += right;
			return *this;
		}

		deque_iterator& operator-=(const difference_type& right) {
			this->ptr -= right;
			return *this;
		}

		template <bool _is_const = is_const,
			typename std::enable_if<_is_const, int>::value = 1>
		deque_iterator& operator=(const deque_iterator& right) {
			this->ptr = right.ptr;
		}

		template <bool _is_const = is_const,
			typename std::enable_if<_is_const, int>::value = 1>
		deque_iterator& operator=(const deque_iterator&& right) {
			this->ptr = right.ptr;
			right = nullptr;
		}

		deque_iterator& operator=(const deque_iterator& right) {
			this->ptr = right.ptr;
		}

		deque_iterator& operator=(const deque_iterator&& right) {
			this->ptr = right.ptr;
			right = nullptr;
		}

		deque_iterator& operator[](const difference_type n) {
			return this->ptr + (n * sizeof(T));
		}

		deque_iterator() {
			this->ptr = nullptr;
		}

		template <bool _is_const = is_const,
			typename std::enable_if<_is_const, int>::value = 1>
		deque_iterator(const deque_iterator<false>& right)
			: ptr(right.ptr) {}

		template <bool _is_const = is_const,
			typename std::enable_if<_is_const, int>::value = 1>
		deque_iterator(const deque_iterator<false>&& right)
			: ptr(right.ptr)
		{
			right = nullptr;
		}

		deque_iterator(const deque_iterator& right)
			: ptr(right.ptr)
		{
		}

		deque_iterator(const deque_iterator&& right)
			: ptr(right.ptr)
		{
			right = nullptr;
		}
	};

	typedef deque_iterator<false> iterator;
	typedef deque_iterator<true> const_iterator;

	// overloaded operators

	// todo: overloaded operators
	// todo: ensure allocator propagation is properly handled

	// member functions

	T& at(size_t n) {
		if (n < this->_size) {
			return this->_buffer[this->_first_index + n];
		}
		else {
			throw std::out_of_range("deque");
		}
	}

	T& operator[] (size_t n) {
		return this->at(n);
	}

	// peeks

	T& peek_back() {
		if (!this->empty()) {
			return this->_buffer[this->_last_index()];
		}
		else {
			throw std::out_of_range("Cannot peek empty deque");
		}
	}

	T& peek_front() {
		if (!this->empty()) {
			return this->_buffer[this->_first_index];
		}
		else {
			throw std::out_of_range("Cannot peek empty deque");
		}
	}

	// pops

	T pop_back() {
		T to_return;

		if (!this->empty()) {
			T *addr = &this->_buffer[this->_last_index()];
			to_return = *addr;

			std::allocator_traits<Allocator>::destroy(this->_allocator, addr);
			this->_size -= 1;

		}
		else {
			throw std::out_of_range("deque is empty");
		}
		
		return to_return;
	}

	T pop_front() {
		T to_return;

		if (!this->empty()) {
			T *addr = &this->_buffer[this->_first_index];
			to_return = *addr;

			std::allocator_traits<Allocator>::destroy(this->_allocator, addr);
			this->_size -= 1;
			this->_first_index += 1;
		}
		else {
			throw std::out_of_range("deque is empty");
		}

		return to_return;
	}

	// pushes

	void push_back(T &elem) {
		// if we have no room at the end, we need to move things around
		if (this->_last_index() == this->_capacity) {
			if (this->_first_index > 1) {
				// move elements forward in the deque
				for (size_t i = 0; i < this->_size; i++) {
					this->_buffer[i + 1] = this->_buffer[this->_first_index + i];
				}
				this->_first_index = 1;
			}
			else {
				// reallocate the deque
				this->_realloc_expand_buffer();
			}
		}

		// now, push back
		this->_size += 1;
		T *addr = &this->_buffer[this->_last_index()];
		std::allocator_traits<Allocator>::construct(this->_allocator, addr, elem);

		return;
	}

	void push_front(T &elem) {
		// if we don't have room at the front, we need to move all elements back
		if (this->_first_index == 1) {
			if (this->_size == this->_capacity) {
				// reallocate the deque
				this->_realloc_expand_buffer();
			}
			else {
				// move elements back
				for (size_t i = this->_last_index(); i > 1; i--) {
					this->_buffer[i + 1] = this->_buffer[i];
				}
				this->_first_index += 1;
			}
		}

		// now, push front
		this->_size += 1;
		this->_first_index -= 1;
		T *addr = &this->_buffer[this->_first_index];
		std::allocator_traits<Allocator>::construct(this->_allocator, addr, elem);

		return;
	}

	// iterators

	iterator& front() {
		if (this->empty())
			throw std::out_of_range("deque::front");
		else
			return this->_buffer[this->_first_index];
	}

	iterator& back() {
		if (this->empty())
			throw std::out_of_range("deque::back");
		else
			return this->_buffer[this->_last_index()];
	}

	iterator begin() {
		if (this->empty())
			return iterator(&this->_buffer[this->_rend_index]);
		else
			return iterator(&this->_buffer[this->_first_index]);
	}

	const_iterator cbegin() const {
		if (this->empty())
			return const_iterator(&this->_buffer[this->_rend_index]);
		else
			return const_iterator(&this->_buffer[this->_first_index]);
	}

	iterator rbegin() {
		if (this->empty())
			return iterator(&this->_buffer[this->_rend_index]);
		else
			return iterator(&this->_buffer[this->_last_index()]);
	}

	const_iterator crbegin() const {
		if (this->empty())
			return const_iterator(&this->_buffer[this->_rend_index]);
		else
			return const_iterator(&this->_buffer[this->_last_index]);
	}

	iterator end() {
		return iterator(&this->_buffer[this->_end_index]);
	}

	const_iterator cend() const {
		return const_iterator(&this->_buffer[this->_end_index]);
	}

	iterator rend() {
		return iterator(&this->_buffer[this->_rend_index]);
	}

	const_iterator crend() const {
		return const_iterator(&this->_buffer[this->_rend_index]);
	}

	deque() {
		this->_allocator = Allocator();
		this->_capacity = 12;
		this->_buffer = std::allocator_traits<Allocator>::allocate(this->_allocator, this->_capacity + 2);

		this->_size = 0;
		this->_first_index = 5;

		this->_rend_index = 0;
		this->_end_index = 9;
	}

	~deque() {
		for (size_t i = this->_first_index; i < this->_last_index(); i++) {
			T *to_destroy = &this->_buffer[i];
			std::allocator_traits<Allocator>::destroy(this->_allocator, to_destroy);
		}
		this->_size = 0;

		std::allocator_traits<Allocator>::deallocate(this->_allocator, this->_buffer, this->_capacity + 2);
		this->_buffer = nullptr;
		this->_capacity = 0;
		this->_size = 0;
		this->_first_index = 0;
	}
};
