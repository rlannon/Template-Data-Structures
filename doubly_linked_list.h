#pragma once

/*

doubly_linked_list.h
Implementation of a doubly-linked list

*/

#include <memory>	// std::allocator
#include <type_traits>	// std::enable_if
#include <iterator>
#include <stdexcept>

template <typename N>
struct dll_node
{
	typedef N value_type;
	typedef N* pointer;
	typedef N& reference;

	N _data;
	dll_node<N>* _next;
	dll_node<N>* _previous;

	dll_node& operator=(const dll_node& r)
	{
		this->_data = r._data;
		this->_next = r._next;
		this->_previous = r._previous;
	}

	dll_node(const N& data, dll_node<N>* prev, dll_node<N>* next)
		: _data(data)
		, _next(next)
		, _previous(prev)
	{
	}

	dll_node(const N& data)
		: dll_node(data, nullptr, nullptr)
	{
	}

	dll_node()
		: dll_node(N(), nullptr, nullptr)
	{
	}

	~dll_node()
	{
	}
};

template < typename T, typename Allocator = std::allocator< dll_node<T> > >
class doubly_linked_list
{
	/*
	
	doubly_linked_list
	Implementation of a doubly-linked list

	Template parameters:
		* T	-	The type contained by the list
		* Allocator	-	The allocator to use; defaults to the STL's default allocator
	
	*/

	Allocator _allocator;

	dll_node<T>* _head;
	dll_node<T>* _tail;

	size_t _size;
public:
	using value_type = T;
	using reference = T&;
	using const_reference = const T&;
	using pointer = T*;
	using const_pointer = const T*;
	using allocator_type = Allocator;

	// capacity functions
	const bool empty() const noexcept;
	const size_t size() const noexcept;
	const size_t max_size() const noexcept;

	allocator_type get_allocator() const noexcept
	{
		// return a copy of the allocator
		return this->_allocator;
	}

	template<bool is_const = false>
	class bidirectional_iterator
	{
		/*
		
		Our iterator class
		
		Utilizes a template parameter to indicate whether it is a const_iterator
		This, alongisde std::enable_if, allows us to avoid code duplication
		
		*/

		friend class doubly_linked_list<T, Allocator>;
		// pointer type
		dll_node<value_type>* ptr;

		// private constructor
		bidirectional_iterator(dll_node<value_type>* other)
			: ptr(other)
		{
		}
	public:
		// definitions
		// utilize std::conditional in case we have a const type (so const_iterator is implemented properly)
		using difference_type = std::ptrdiff_t;
		using value_type = typename std::conditional< is_const, const T, T>::type;
		using pointer = T * ;
		using reference = T & ;
		using const_pointer = const T*;
		using const_reference = const T&;
		using iterator_category = std::bidirectional_iterator_tag;

		// public methods
		bool operator==(const bidirectional_iterator& right)
		{
			return this->ptr == right.ptr;
		}

		bool operator!=(const bidirectional_iterator& right)
		{
			return this->ptr != right.ptr;
		}

		reference operator*()
		{
			// return the node::_data member (not the node)
			return this->ptr->_data;
		}

		pointer operator->()
		{
			// return the address of the node::_data member (not the address of the node)
			return &this->ptr->_data;
		}

		bidirectional_iterator<is_const>& operator++() 
		{
			if (this->ptr)
			{
				this->ptr = this->ptr->_next;
				return *this;
			}
			else
			{
				throw std::out_of_range("doubly-linked list");
			}
		}

		bidirectional_iterator<is_const> operator++(int)
		{
			if (this->ptr)
			{
				bidirectional_iterator to_return(*this);
				this->ptr = this->ptr->_next;
				return to_return;
			}
			else
			{
				throw std::out_of_range("doubly-linked list");
			}
		}

		bidirectional_iterator<is_const>& operator--()
		{
			if (this->ptr)
			{
				this->ptr = this->ptr->_previous;
				return *this;
			}
			else
			{
				throw std::out_of_range("doubly-linked list");
			}
		}

		bidirectional_iterator<is_const> operator--(int)
		{
			if (this->ptr)
			{
				bidirectional_iterator to_return(*this);
				this->ptr = this->ptr->_previous;
				return to_return;
			}
			else
			{
				throw std::out_of_range("doubly-linked list");
			}
		}

		template<bool _is_const = is_const,
			typename std::enable_if<_is_const, int>::type = 1>
			bidirectional_iterator& operator=(const bidirectional_iterator<false>& right)
		{
			// specialized copy-assignment operator
			this->ptr = right.ptr;
			return *this;
		}

		template<bool _is_const = is_const,
			typename std::enable_if<_is_const, int>::type = 1>
			bidirectional_iterator& operator=(const bidirectional_iterator<false>&& right)
		{
			// specialized move assignment operator
			this->ptr = right.ptr;
			right.ptr = nullptr;
			right = nullptr;
			return *this;
		}

		bidirectional_iterator<is_const>& operator=(const bidirectional_iterator& right)
		{
			// general copy-assignment operator
			this->ptr = right.ptr;
			return *this;
		}

		bidirectional_iterator<is_const>& operator=(const bidirectional_iterator&& right)
		{
			// general move assignment operator
			this->ptr = right.ptr;
			right.ptr = nullptr;
			right = nullptr;
			return *this;
		}

		// constructors, destructor
		bidirectional_iterator() noexcept : ptr{} {};
		
		template <bool _is_const = is_const,
			typename std::enable_if<_is_const, int>::type = 1>
		bidirectional_iterator(const bidirectional_iterator<false>& it)
			: ptr(it.ptr)
		{
			// specialized constructor to allow a const_iterator to be initialized with an iterator
		}

		template <bool _is_const = is_const,
			typename std::enable_if<_is_const, int>::type = 1>
			bidirectional_iterator(const bidirectional_iterator<false>&& it)
			: ptr(it.ptr)
		{
			// specialized move constructor for the same purpose
			it.ptr = nullptr;
			it = nullptr;
		}

		bidirectional_iterator(const bidirectional_iterator& it)
			: ptr(it.ptr)
		{
			// general copy constructor
		}

		bidirectional_iterator(const bidirectional_iterator&& it)
			: ptr(it.ptr)
		{
			// general move constructor
			it.ptr = nullptr;
			it = nullptr;
		}
	};

	// define our iterators
	typedef bidirectional_iterator<false> iterator;
	typedef bidirectional_iterator<true> const_iterator;
	typedef std::reverse_iterator<bidirectional_iterator<false> > reverse_iterator;
	typedef std::reverse_iterator<bidirectional_iterator<true> > const_reverse_iterator;	// reverse iterators utilize std::reverse_iterator with our iterator as a template parameter

	// modifiers
	void push_back(const T& val)
	{
		auto loc = std::allocator_traits<Allocator>::allocate(this->_allocator, 1);
		std::allocator_traits<Allocator>::construct(this->_allocator, loc, val);

		if (this->empty())
		{
			this->_head = loc;
		}
		else if (this->_size == 1)
		{
			this->_tail = loc;
			this->_tail->_previous = this->_head;
			this->_head->_next = this->_tail;
		}
		else
		{
			this->_tail->_next = loc;
			loc->_previous = this->_tail;
			this->_tail = loc;
		}

		this->_size += 1;
	}

	void pop_back()
	{
		if (this->empty())
		{
			throw std::out_of_range("doubly-linked list");
		}
		else if (this->_size == 1)
		{
			// if the size is 1, our head node is the only one (tail is nullptr)
			// as such, we should pop_front
			this->pop_front();
		}
		else
		{
			// otherwise, we have a node at 'tail' we can pop
			auto loc = this->_tail;
			this->_tail = this->_tail->_previous;	// move the tail node back one
			this->_tail->_next = nullptr;	// ensure nothing comes after it

			// destroy and deallocate the node
			std::allocator_traits<Allocator>::destroy(this->_allocator, loc);
			std::allocator_traits<Allocator>::deallocate(this->_allocator, loc, 1);

			this->_size -= 1;
		}
	}

	void push_front(const T& val);
	void pop_front();

	void insert(const_iterator position, const T& val)
	{
		// insert a single element at 'position', initializing with val
		// this will move elements behind it back

		auto new_element_pointer = std::allocator_traits<Allocator>::allocate(this->_allocator, 1);
		std::allocator_traits<Allocator>::construct(this->_allocator, new_element_pointer, val);

		// hold the element before 'position' temporarily so we don't lose it
		auto previous_position = position.ptr->_previous;

		/*

		Update the relationships:
			* the new element points to the element before 'position'
			* the new element points to the element at 'position'
			* the element before 'position' is the new element

		*/

		new_element_pointer->_previous = previous_position;
		new_element_pointer->_next = position.ptr;
		position.ptr->_previous = new_element_pointer;

		// increment the size
		this->_size++;

		return;
	}

	void insert(const_iterator position, size_t n, const T& val)
	{
		// fills the list with n elements (each initialized by val) at position

		for (size_t i = 0; i < n; i++)
		{
			this->insert(position, val);
		}

		return;
	}

	template <typename InputIterator>
	void insert(const_iterator position, InputIterator first, InputIterator last)
	{
		// insert elements by iterator from first to last at position

		// todo: iterator insertion

		return;
	}


	void insert(const_iterator position, T&& val)
	{
		/*
		
		insert
		Performs a move insertion at 'position' of value 'val'

		The value inserted will be the value at 'position', moving elements currently at and after the iterator back

		*/
		
		auto new_element_pointer = std::allocator_traits<Allocator>::allocator(this->_allocator, 1);
		std::allocator_traits<Allocator>::construct(this->_allocator, new_element_pointer, val);

		// hold the element before 'position' temporarily
		auto previous_position = position.ptr->_previous;

		// update the relationships in the same way as we do in copy insertion
		new_element_pointer->_previous = previous_position;
		new_element_pointer->_next = position.ptr;
		position.ptr->_previous = new_element_pointer;

		// ensure val is set to nullptr, as is required with move semantics
		val = nullptr;

		// increment the size
		this->_size++;

		return;
	}

	void insert(const_iterator position, std::initializer_list<T> il)
	{
		/*
		
		insert
		Inserts elements contained within the list 'li' at 'position'

		This will call 'insert' for each element

		*/

		for (auto it = il.begin(); it != il.end(); it++)
		{
			this->insert(position, *it);
		}

		return;
	}

	// iterators and accession
	
	iterator begin() noexcept
	{
		return iterator(this->_head);
	}

	iterator end() noexcept
	{
		return iterator(nullptr);
	}
	
	const_iterator cbegin() const noexcept
	{
		return const_iterator(this->_head);
	}

	const_iterator cend() const noexcept
	{
		return const_iterator(nullptr);
	}

	reverse_iterator rbegin() noexcept
	{
		return reverse_iterator(bidirectional_iterator<false>(this->_tail));
	}

	reverse_iterator rend() noexcept
	{
		return reverse_iterator(bidirectional_iterator<false>(nullptr));
	}

	const_reverse_iterator crbegin() const noexcept
	{
		return reverse_iterator(bidirectional_iterator<true>(this->_tail));
	}

	const_reverse_iterator crend() const noexcept
	{
		return reverse_iterator(bidirectional_iterator<true>(nullptr));
	}

	reference front()
	{
		if (this->_head)
		{
			return this->_tail->_data;
		}
		else {
			throw std::out_of_range("doubly-linked list");
		}
	}

	const_reference front() const
	{
		if (this->_tail)
		{
			return this->_tail->_data;
		}
		else {
			throw std::out_of_range("doubly-linked list");
		}
	}

	T& back();

	// constructors and destructor
	doubly_linked_list(const Allocator& alloc);
	doubly_linked_list();
	doubly_linked_list(size_t n, const Allocator& alloc);
	doubly_linked_list(size_t n, const T& val, const Allocator& alloc);
	doubly_linked_list(const doubly_linked_list<T, Allocator>& l);
	doubly_linked_list(const doubly_linked_list<T, Allocator>& l, const Allocator& alloc);
	doubly_linked_list(const doubly_linked_list<T, Allocator>&& l);
	doubly_linked_list(const doubly_linked_list<T, Allocator>&& l, const Allocator& alloc);
	doubly_linked_list(std::initializer_list<T> il, const Allocator& alloc);
	
	~doubly_linked_list();
};

template <typename T, typename Allocator>
const bool doubly_linked_list<T, Allocator>::empty() const noexcept
{
	return this->_size == 0;
}

template <typename T, typename Allocator>
const size_t doubly_linked_list<T, Allocator>::size() const noexcept
{
	return this->_size;
}

template <typename T, typename Allocator>
const size_t doubly_linked_list<T, Allocator>::max_size() const noexcept
{
	return std::allocator_traits<Allocator>::max_size(this->_allocator);
}

// operators
	
// todo: copy and move assignment operators; assign with initializer_list
// todo: ensure allocator propagation is handled properly

template <typename T, typename Allocator>
void doubly_linked_list<T, Allocator>::push_front(const T& val)
{
	auto loc = std::allocator_traits<Allocator>::allocate(this->_allocator, 1);
	std::allocator_traits<Allocator>::construct(this->_allocator, loc, val);

	if (this->empty())
	{
		this->_head = loc;
	}
	else
	{
		this->_head->_previous = loc;
		loc->_next = this->_head;
		this->_head = loc;
	}

	this->_size += 1;
}

template <typename T, typename Allocator>
void doubly_linked_list<T, Allocator>::pop_front()
{
	if (this->empty())
	{
		throw std::out_of_range("doubly-linked list");
	}
	else 
	{
		// reset the head node
		auto loc = this->_head;
			
		// if we only have two nodes, we need to set the tail to nullptr so we don't have a node pointing to itself
		if (this->_head->_next == this->_tail)
		{
			this->_tail = nullptr;
		}

		this->_head = this->_head->_next;	// point _head to the element after it
		this->_head->_previous = nullptr;	// ensure the _head node points to nothing before it
		this->_size -= 1;

		// destroy and deallocate what was at 'head'
		std::allocator_traits<Allocator>::destroy(this->_allocator, loc);
		std::allocator_traits<Allocator>::deallocate(this->_allocator, loc, 1);
	}
}

// constructors, destructor

template <typename T, typename Allocator>
doubly_linked_list<T, Allocator>::doubly_linked_list(const Allocator& alloc): doubly_linked_list()
{
	this->_allocator = alloc;
}

template <typename T, typename Allocator>
doubly_linked_list<T, Allocator>::doubly_linked_list()
{
	this->_allocator = Allocator();
	this->_head = nullptr;
	this->_tail = nullptr;
	this->_size = 0;
}

template <typename T, typename Allocator>
doubly_linked_list<T, Allocator>::doubly_linked_list(size_t n, const Allocator& alloc)
{
	/*
	
	constructor
	Constructs the doubly-linked list be calling the default constructor for 'n' elements utilizing the specified allocator
	
	*/

	this->_allocator = alloc;
	this->_head = nullptr;
	this->_tail = nullptr;
	this->_size = 0;

	dll_node<T>* previous_pointer = nullptr;

	for (size_t i = 0; i < n; i++)
	{
		// allocate and construct a new (default) item
		auto elem_pointer = std::allocator_traits<Allocator>::allocate(this->_allocator, 1);
		std::allocator_traits<Allocator>::construct(this->_allocator, elem_pointer);

		// set our node
		if (previous_pointer)
		{
			// if we have a previous node, set this as the next one;
			previous_pointer->_next = elem_pointer;
		}
		else {
			// otherwise, set this as 'previous_pointer' and set the head node
			this->_head = elem_pointer;
		}	// todo: this could be optimized slightly by having a check _before_ the loop

		// no matter whether we had a node before this one, we need to assign _tail and previous_pointer
		previous_pointer = elem_pointer;
		this->_tail = previous_pointer;
		this->_size++;
	}
}

template <typename T, typename Allocator>
doubly_linked_list<T, Allocator>::doubly_linked_list(size_t n, const T& val, const Allocator& alloc)
{
	/*
	
	constructor
	Similar to the above constructor, except that it initializes all elements with 'val'
	
	*/

	this->_allocator = alloc;
	this->_head = nullptr;
	this->_tail = nullptr;
	this->_size = 0;

	dll_node<T>* previous_pointer = nullptr;

	for (size_t i = 0; i < n; i++)
	{
		// allocate and construct the node
		auto elem_pointer = std::allocator_traits<Allocator>::allocate(this->_allocator, 1);
		std::allocator_traits<Allocator>::construct(this->_allocator, elem_pointer, val);

		// set our node
		if (previous_pointer)
		{
			previous_pointer->_next = elem_pointer;
		}
		else
		{
			this->_head = elem_pointer;
		}

		// updates that need to happen regardless of whether we had a previous node
		previous_pointer = elem_pointer;
		this->_tail = previous_pointer;
		this->size++;
	}
}

// todo: copy constructors
template <typename T, typename Allocator>
doubly_linked_list<T, Allocator>::doubly_linked_list(const doubly_linked_list<T, Allocator>& l)
{
	/*
	
	constructor
	Copy constructor with default allocator

	This only needs to assign a few things -- the list itself doesn't contain elements, just pointers

	*/

	this->_allocator = l._allocator;
	this->_head = l._head;
	this->_tail = l._tail;
	this->_size = l._size;
}

template <typename T, typename Allocator>
doubly_linked_list<T, Allocator>::doubly_linked_list(const doubly_linked_list<T, Allocator>& l, const Allocator& alloc)
{
	// copy constructor with custom allocator
	// todo
}

template <typename T, typename Allocator>
doubly_linked_list<T, Allocator>::doubly_linked_list(const doubly_linked_list<T, Allocator>&& l)
{
	/*

	constructor
	Move constructor with default allocator

	*/

	this->_allocator = l._allocator;
	this->_head = l._head;
	this->_tail = l._tail;
	this->_size = l._size;

	l = nullptr;
}

template <typename T, typename Allocator>
doubly_linked_list<T, Allocator>::doubly_linked_list(const doubly_linked_list<T, Allocator>&& l, const Allocator& alloc)
{
	// move constructor with custom allocator
	// todo
}

template <typename T, typename Allocator>
doubly_linked_list<T, Allocator>::doubly_linked_list(std::initializer_list<T> il, const Allocator& alloc)
{
	/*
	
	constructor
	Constructs the list with an initializer list

	*/

	this->_allocator = alloc;
	this->_head = nullptr;
	this->_tail = nullptr;
	this->_size = 0;

	dll_node<T>* previous_pointer = nullptr;

	// iterate through the initializer list, allocating and constructing new elements for each
	for (auto it = il.begin(); it != il.end(); it++)
	{
		auto elem_pointer = std::allocator_traits<Allocator>::allocate(this->_allocator, 1);
		std::allocator_traits<Allocator>::construct(this->_allocator, elem_pointer, *it);

		if (previous_pointer)
		{
			previous_pointer->_next = elem_pointer;
		}
		else
		{
			this->_head = elem_pointer;
		}

		previous_pointer = elem_pointer;
		this->_tail = previous_pointer;
		this->_size++;
	}
}

template <typename T, typename Allocator>
doubly_linked_list<T, Allocator>::~doubly_linked_list()
{
	/*
	
	destructor
	Walks through the list, calling the destroying and deallocating the objects in the list

	*/

	// walk through the list and call the destructor for every element
	auto current_pointer = this->_head;

	// if we have a node at the pointer, continue; else, we don't need to do anything (the list is empty)
	while (current_pointer)
	{
		// get the next element (to keep track of it)
		auto next_pointer = current_pointer->_next;

		// destroy and deallocate the current element
		std::allocator_traits<Allocator>::destroy(this->_allocator, current_pointer);
		std::allocator_traits<Allocator>::deallocate(this->_allocator, current_pointer, 1);

		// update the current pointer and decrement the size
		current_pointer = next_pointer;
		this->_size--;
	}
}
