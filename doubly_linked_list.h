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

		node(const N& data) : _data(data)
		{
			this->_next = nullptr;
			this->_previous = nullptr;
		}

		node() : node(data())
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

	const bool empty() const noexcept
	{
		return this->_size == 0;
	}

	const size_t size() const noexcept
	{
		return this->_size;
	}

	const size_t max_size() const noexcept
	{
		return std::allocator_traits<Allocator>::max_size(this->_allocator);
	}

	allocator_type get_allocator() const noexcept
	{
		// return a copy of the allocator
		return this->_allocator;
	}

	// the list iterator
	// utilizes a template to avoid code duplication
	template<typename iter_type>
	class list_iterator
	{
		// a pointer to a node<T> or const node<T>
		iter_type* ptr;

		list_iterator(iter_type* ptr)
		{
			// private constructor
			this->ptr = ptr;
		}
	public:
		// define the iterator traits
		typedef typename iter_type::value_type value_type;
		typedef std::bidirectional_iterator_tag iterator_category;
		typedef ptrdiff_t difference_type;
		typedef typename iter_type::pointer pointer;
		typedef typename iter_type::reference reference;

		// overload the operators
		bool operator==(const list_iterator& right)
		{
			return this->ptr == right.ptr;
		}

		bool operator!=(const list_iterator& right)
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
			&return this->ptr->_data;
		}
		
		list_iterator& operator++()
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

		list_iterator operator++(int)
		{
			if (this->ptr)
			{
				list_iterator to_return(*this);
				this->ptr = this->ptr->_next;
				return to_return;
			}
			else
			{
				throw std::out_of_range("doubly-linked list");
			}
		}

		list_iterator& operator--()
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

		list_iterator operator--(int)
		{
			if (this->ptr)
			{
				list_iterator to_return(*this);
				this->ptr = this->ptr->_previous;
				return to_return;
			}
			else
			{
				throw std::out_of_range("doubly-linked list");
			}
		}

		list_iterator& operator=(const list_iterator& right)
		{
			this->ptr = right.ptr;
			return *this;
		}

		// constructors

		list_iterator(const list_iterator& it)
		{
			this->ptr = it.ptr;
		}

		list_iterator()
		{
			this->ptr = nullptr;
		}

		~list_iterator()
		{
		}
	};

	// define iterator and const_iterator
	typedef list_iterator< node<T> > iterator;
	typedef list_iterator< const node<T> > const_iterator;

	// operators
	
	// todo: copy and move assignment operators; assign with initializer_list
	// todo: ensure allocator propagation is handled properly

	// container methods
	void push_back(const T& val)
	{
		// allocate and construct the node
		node<T>* loc = std::allocator_traits<Allocator>::allocate(this->_allocator, 1);
		std::allocator_traits<Allocator>::construct(this->_allocator, loc, val);
		
		if (this->empty())
		{
			this->_head = to_append;
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
			node<T>* loc = this->_tail;
			this->_tail = this->_tail->_previous;	// move the tail node back one
			this->_tail->_next = nullptr;	// ensure nothing comes after it

			// destroy and deallocate the node
			std::allocator_traits<Allocator>::destroy(this->_allocator, loc);
			std::allocator_traits<Allocator>::deallocate(this->_allocator, loc, 1);

			this->_size -= 1;
		}
	}

	void push_front(const T& val)
	{
		node<T>* loc = std::allocator_traits<Allocator>::allocate(this->_allocator, 1);
		std::allocator_traits<Allocator>::construct(this->_allocator, loc, val);

		if (this->empty())
		{
			this->_head = to_append;
		}
		else
		{
			this->_head->_previous = to_prepend;
			to_prepend->_next = this->_head;
			this->_head = to_prepend;
		}

		this->_size += 1;
	}

	void pop_front()
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

	void insert(const_iterator position, const value_type& val)
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

	void insert(const_iterator position, size_t n, const value_type& val)
	{
		// fills the list with n elements (each initialized by val) at position

		// todo: list fill

		return;
	}

	template <class InputIterator>
	void insert(const_iterator position, InputIterator first, InputIterator last)
	{
		// insert elements by iterator from first to last at position
		
		// todo: iterator insertion

		return;
	}

	void insert(const_iterator position, value_type&& val)
	{
		// move insertion

		// todo: move insertion

		return;
	}

	void insert(const_iterator position, std::initializer_list<value_type> li)
	{
		// insert by initializer list

		// todo: initializer list insertion

		return;
	}

	// iterators

	iterator begin()
	{
		return iterator(this->_head);
	}

	const_iterator cbegin() const
	{
		return const_iterator(this->_head);
	}

	iterator end()
	{
		return iterator(nullptr);
	}

	const_iterator cend() const
	{
		return const_iterator(nullptr);
	}

	iterator rbegin()
	{
		// if we only have one element, it is at _head
		if (this->_size == 1)
			return iterator(this->_head);
		else
			return iterator(this->_tail);
	}

	const_iterator crbegin() const
	{
		if (this->_size == 1)
			return const_iterator(this->_head);
		else
			return const_iterator(this->_tail);
	}

	iterator rend()
	{
		return iterator(nullptr);
	}

	const_iterator crend()
	{
		return const_iterator(nullptr);
	}

	// constructor, destructor

	explicit doubly_linked_list(const allocator_type& alloc): doubly_linked_list()
	{
		this->_allocator = alloc;
	}

	doubly_linked_list()
	{
		this->_allocator = Allocator();
		this->_head = nullptr;
		this->_tail = nullptr;
		this->_size = 0;
	}

	// todo: fill constructors
	explicit doubly_linked_list(size_t n, const allocator_type& alloc = allocator_type())
	{
		// fill constructor with default construction of 'val'
	}

	doubly_linked_list(size_t n, const value_type& val, const allocator_type& alloc = allocator_type())
	{
		// fill constructor with 'val'
	}

	// todo: copy constructors
	doubly_linked_list(const doubly_linked_list& l)
	{
		// copy constructor with default allocator
	}

	doubly_linked_list(const doubly_linked_list& l, const allocator_type& alloc)
	{
		// copy constructor with custom allocator
	}

	// todo: move constructors
	doubly_linked_list(const doubly_linked_list&& l)
	{
		// move constructor with default allocator
	}

	doubly_linked_list(const doubly_linked_list&& l, const allocator_type& alloc)
	{
		// move constructor with custom allocator
	}

	// todo: initializer list
	doubly_linked_list(std::initializer_list<value_type> il, const allocator_type& alloc = allocator_type())
	{
		// construct with initializer list
	}

	// destructor

	~doubly_linked_list()
	{
		// walk through the list and call the destructor for every element
	}
};
