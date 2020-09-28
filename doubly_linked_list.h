#pragma once

/*

doubly_linked_list.h
Implementation of a doubly-linked list

*/

#include <memory>
#include <stdexcept>

template <typename N>
struct node;

template < typename T, typename Allocator = std::allocator< node<T> > >
class doubly_linked_list
{
	template <typename N>
	struct node
	{
		typedef N value_type;
		typedef N* pointer;
		typedef N& reference;

		N _data;
		node<N>* _next;
		node<N>* _previous;

		node& operator=(const node& r)
		{
			this->_data = r._data;
			this->_next = r._next;
			this->_previous = r._previous;
		}

		node(const N& data, node<N>* prev, node<N>* next):
			_data(data),
			_next(next),
			_previous(prev)
		{
		}

		node(const N& data) : node(data, nullptr, nullptr)
		{
		}

		node() : node( N(), nullptr, nullptr )
		{
		}

		~node()
		{
		}
	};

	Allocator _allocator;

	node<T>* _head;
	node<T>* _tail;

	size_t _size;
public:
	typedef T value_type;
	typedef T& reference;
	typedef T* pointer;
	typedef Allocator allocator_type;

	// capacity functions
	const bool empty() const noexcept;
	const size_t size() const noexcept;
	const size_t max_size() const noexcept;

	allocator_type get_allocator() const noexcept;

	// the list iterator
	// utilizes a template to avoid code duplication
	template<typename iter_type>
	class bidirectional_iterator
	{
		// a pointer to a node<T> or node<const T>
		iter_type* ptr;

		bidirectional_iterator(iter_type* ptr);
	public:
		// define the iterator traits
		typedef typename iter_type::value_type value_type;
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef ptrdiff_t difference_type;
		typedef typename iter_type::pointer pointer;
		typedef typename iter_type::reference reference;

		// overload the operators
		bool operator==(const bidirectional_iterator& right);
		bool operator!=(const bidirectional_iterator& right);
		T& operator*();
		T* operator->();
		bidirectional_iterator& operator++();
		bidirectional_iterator operator++(int);
		bidirectional_iterator& operator--();
		bidirectional_iterator operator--(int);
		bidirectional_iterator& operator=(const bidirectional_iterator& right);

		// constructors
		bidirectional_iterator(const bidirectional_iterator& it);
		bidirectional_iterator();
		~bidirectional_iterator();
	};

	template<typename iter_type>
	class reverse_list_iterator
	{
		iter_type* ptr;
		reverse_list_iterator(iter_type* ptr);
	public:
		typedef typename iter_type::value_type value_type;
		
		reverse_list_iterator();
		~reverse_list_iterator();
	};

	// define our iterators
	typedef bidirectional_iterator< node<T> > iterator;
	typedef bidirectional_iterator< node<const T> > const_iterator;
	typedef reverse_list_iterator< node<T> > reverse_iterator;
	typedef reverse_list_iterator< node<cosnt T> > const_reverse_iterator;

	// modifiers
	void push_back(const T& val);
	void pop_back();
	void push_front(const T& val);
	void pop_front();

	void insert(const_iterator position, const T& val);
	void insert(const_iterator position, size_t n, const T& val);

	template <typename InputIterator>
	void insert(const_iterator position, InputIterator first, InputIterator last);

	void insert(const_iterator position, T&& val);
	void insert(const_iterator position, std::initializer_list<T> li);

	// todo: more methods should be supported

	// iterators and accession
	iterator begin() noexcept;
	iterator end() noexcept;
	const_iterator cbegin() const noexcept;
	const_iterator cend() const noexcept;

	reverse_iterator rbegin() noexcept;
	reverse_iterator rend() noexcept;
	const_reverse_iterator crbegin() const noexcept;
	const_reverse_iterator crend() const noexcept;

	T& front();
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

template <typename T, typename Allocator>
doubly_linked_list<T, Allocator>::allocator_type doubly_linked_list<T, Allocator>::get_allocator() const noexcept
{
	// return a copy of the allocator
	return this->_allocator;
}

// operators
	
// todo: copy and move assignment operators; assign with initializer_list
// todo: ensure allocator propagation is handled properly

// container methods
template <typename T, typename Allocator>
void doubly_linked_list<T, Allocator>::push_back(const T& val)
{
doubly_linked_list<T, Allocator>::	// allocate and construct the node
	node<T>* loc = std::allocator_traits<Allocator>::allocate(this->_allocator, 1);
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

template <typename T, typename Allocator>
void doubly_linked_list<T, Allocator>::pop_back()
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
		node<T>* loc = this->_tail;
		this->_tail = this->_tail->_previous;	// move the tail node back one
		this->_tail->_next = nullptr;	// ensure nothing comes after it

		// destroy and deallocate the node
		std::allocator_traits<Allocator>::destroy(this->_allocator, loc);
		std::allocator_traits<Allocator>::deallocate(this->_allocator, loc, 1);

		this->_size -= 1;
	}
}

template <typename T, typename Allocator>
void doubly_linked_list<T, Allocator>::push_front(const T& val)
{
	node<T>* loc = std::allocator_traits<Allocator>::allocate(this->_allocator, 1);
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
		node<T>* loc = this->_head;
			
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

// insert method

template <typename T, typename Allocator>
void doubly_linked_list<T, Allocator>::insert(const_iterator position, const T& val)
{
	// insert a single element at 'position', initializing with val
	// this will move elements behind it back

	// todo: allow move construction
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

	return;
}

template <typename T, typename Allocator>
void doubly_linked_list<T, Allocator>::insert(const_iterator position, size_t n, const T& val)
{
	// fills the list with n elements (each initialized by val) at position

	// todo: list fill

	return;
}

template <typename T, typename Allocator>
template <typename InputIterator>
void doubly_linked_list<T, Allocator>::insert(const_iterator position, InputIterator first, InputIterator last)
{
	// insert elements by iterator from first to last at position
		
	// todo: iterator insertion

	return;
}

template <typename T, typename Allocator>
void doubly_linked_list<T, Allocator>::insert(const_iterator position, T&& val)
{
	// move insertion

	// todo: move insertion

	return;
}

template <typename T, typename Allocator>
void doubly_linked_list<T, Allocator>::insert(const_iterator position, std::initializer_list<value_type> li)
{
	// insert by initializer list

	// todo: initializer list insertion

	return;
}

// iterators

template <typename T, typename Allocator>
doubly_linked_list<T, Allocator>::iterator doubly_linked_list<T, Allocator>::begin() noexcept
{
	return iterator(this->_head);
}

template <typename T, typename Allocator>
doubly_linked_list<T, Allocator>::const_iterator doubly_linked_list<T, Allocator>::cbegin() const noexcept
{
	return const_iterator(this->_head);
}

template <typename T, typename Allocator>
doubly_linked_list<T, Allocator>::iterator doubly_linked_list<T, Allocator>::end() noexcept
{
	return iterator(nullptr);
}

template <typename T, typename Allocator>
doubly_linked_list<T, Allocator>::const_iterator doubly_linked_list<T, Allocator>::cend() const noexcept
{
	return const_iterator(nullptr);
}

// constructor, destructor

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

// todo: fill constructors

template <typename T, typename Allocator>
doubly_linked_list<T, Allocator>::doubly_linked_list(size_t n, const Allocator& alloc)
{
	// fill constructor with default construction of 'val'
}

template <typename T, typename Allocator>
doubly_linked_list<T, Allocator>::doubly_linked_list(size_t n, const T& val, const Allocator& alloc)
{
	// fill constructor with 'val'
}

// todo: copy constructors
template <typename T, typename Allocator>
doubly_linked_list<T, Allocator>::doubly_linked_list(const doubly_linked_list<T, Allocator>& l)
{
	// copy constructor with default allocator
}

template <typename T, typename Allocator>
doubly_linked_list<T, Allocator>::doubly_linked_list(const doubly_linked_list<T, Allocator>& l, const Allocator& alloc)
{
	// copy constructor with custom allocator
}

// todo: move constructors
template <typename T, typename Allocator>
doubly_linked_list<T, Allocator>::doubly_linked_list(const doubly_linked_list<T, Allocator>&& l)
{
	// move constructor with default allocator
}

template <typename T, typename Allocator>
doubly_linked_list<T, Allocator>::doubly_linked_list(const doubly_linked_list<T, Allocator>&& l, const Allocator& alloc)
{
	// move constructor with custom allocator
}

// todo: initializer list
template <typename T, typename Allocator>
doubly_linked_list<T, Allocator>::doubly_linked_list(std::initializer_list<T> il, const Allocator& alloc)
{
	// construct with initializer list
}

// destructor

template <typename T, typename Allocator>
doubly_linked_list<T, Allocator>::~doubly_linked_list()
{
	// walk through the list and call the destructor for every element
}

/*

Forward iterator implementation

*/

// overload the operators
template <typename T, typename Allocator>
template<typename iter_type>
bool doubly_linked_list<T, Allocator>::bidirectional_iterator<iter_type>::operator==(const bidirectional_iterator& right)
{
	return this->ptr == right.ptr;
}

template <typename T, typename Allocator>
template<typename iter_type>
bool  doubly_linked_list<T, Allocator>::bidirectional_iterator<iter_type>::operator!=(const bidirectional_iterator& right)
{
	return this->ptr != right.ptr;
}

template <typename T, typename Allocator>
template<typename iter_type>
T& doubly_linked_list<T, Allocator>::bidirectional_iterator<iter_type>::operator*()
{
	// return the node::_data member (not the node)
	return this->ptr->_data;
}

template <typename T, typename Allocator>
template<typename iter_type>
T* doubly_linked_list<T, Allocator>::bidirectional_iterator<iter_type>::operator->()
{
	// return the address of the node::_data member (not the address of the node)
	return &this->ptr->_data;
}

template <typename T, typename Allocator>
template<typename iter_type>
doubly_linked_list<T, Allocator>::bidirectional_iterator<iter_type>& doubly_linked_list<T, Allocator>::bidirectional_iterator<iter_type>::operator++()
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

template <typename T, typename Allocator>
template<typename iter_type>
doubly_linked_list<T, Allocator>::bidirectional_iterator<iter_type> doubly_linked_list<T, Allocator>::bidirectional_iterator<iter_type>::operator++(int)
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

template <typename T, typename Allocator>
template<typename iter_type>
doubly_linked_list<T, Allocator>::bidirectional_iterator<iter_type>& doubly_linked_list<T, Allocator>::bidirectional_iterator<iter_type>::operator--()
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

template <typename T, typename Allocator>
template<typename iter_type>
doubly_linked_list<T, Allocator>::bidirectional_iterator<iter_type> doubly_linked_list<T, Allocator>::bidirectional_iterator<iter_type>::operator--(int)
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

template <typename T, typename Allocator>
template<typename iter_type>
doubly_linked_list<T, Allocator>::bidirectional_iterator<iter_type>& doubly_linked_list<T, Allocator>::bidirectional_iterator<iter_type>::operator=(const bidirectional_iterator& right)
{
	this->ptr = right.ptr;
	return *this;
}

// constructors

template <typename T, typename Allocator>
template<typename iter_type>
doubly_linked_list<T, Allocator>::bidirectional_iterator<iter_type>::bidirectional_iterator(iter_type* ptr): ptr(ptr)
{
}

template <typename T, typename Allocator>
template<typename iter_type>
doubly_linked_list<T, Allocator>::bidirectional_iterator<iter_type>::bidirectional_iterator(const bidirectional_iterator& it): ptr(it.ptr)
{
}

template <typename T, typename Allocator>
template<typename iter_type>
doubly_linked_list<T, Allocator>::bidirectional_iterator<iter_type>::bidirectional_iterator()
{
	this->ptr = nullptr;
}

template <typename T, typename Allocator>
template<typename iter_type>
doubly_linked_list<T, Allocator>::bidirectional_iterator<iter_type>::~bidirectional_iterator()
{
}
