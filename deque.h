/*

Algorithms and Data Structures
Copyright 2019 Riley Lannon
deque.h

Implements a double-ended queue

*/

#pragma once

#include <initializer_list>
#include <memory>
#include <stdexcept>

template <typename T, typename Alloc = std::allocator<T>>
class deque
{
    Alloc alloc;
    size_t _size;
    size_t _capacity;

    T* buffer;  // a pointer to the buffer's beginning
    T* first_elem;  // a pointer to the first element
public:
    // implement a random-access iterator
    class iterator
    {
        T* ptr;
    public:
        // iterator traits
        typedef T value_type;
        typedef std::random_access_iterator_tag iterator_category;
        typedef ptrdiff_t difference_type;
        typedef T* pointer;
        typedef T& reference;

        /*

        Overloaded Operators

        */

        // inequalities
        bool operator==(const iterator right);
        bool operator!=(const iterator right);
        bool operator>(const iterator right);
        bool operator>=(const iterator right);
        bool operator<(const iterator right);
        bool operator<=(const iterator right);

        // dereferencing
        T& operator*()
        {
            return *this->ptr;
        }

        T* operator->()
        {
            return this->ptr;
        }

        // offset
        T& operator[](difference_type idx)
        {
            return this->ptr[idx];
        }

        // arithmetic
        iterator& operator++()
        {
            // prefix ++ operator
            this->ptr++;
            return *this;
        }

        iterator operator++(int)
        {
            // postfix ++ operator
            auto to_return = *this;
            this->ptr++;
            return to_return;
        }

        iterator& operator--()
        {
            // prefix -- operator
            this->ptr--;
            return *this;
        }

        iterator operator--(int)
        {
            // postfix -- operator
            auto to_return = *this;
            this->ptr--;
            return to_return;
        }

        // + with int, iterator
        iterator& operator+(difference_type right)
        {
            this->ptr += right;
            return *this;
        }

        iterator& operator+(iterator right)
        {
            this->ptr += right->ptr;
            return *this;
        }

        // - with int, iterator
        iterator& operator-(difference_type right)
        {
            this->ptr += right;
            return *this;
        }

        iterator& operator-(iterator right)
        {
            this->ptr += right->ptr;
            return *this;
        }

        // compound assigment with + and -
        iterator& operator+=(difference_type right);
        iterator& operator-=(difference_type right);

        // TODO: overload std::swap(a, b)

        // constructors, destructors
        iterator(T* ptr)
        {
            this->ptr = ptr;
        }

        iterator()
        {
            this->ptr = nullptr;
        }

        ~iterator()
        {

        }
    };

    // iterator-returning functions
    iterator begin() const { return iterator(&this->buffer[0]); }
    iterator back() const { return iterator(&this->buffer[this->_size - 1]); }
    iterator end() const { return iterator(&this->buffer[this->_size]); }

    void push_back(T to_push);
    void push_front(T to_push);

    T pop_back();
    T pop_front();

    explicit deque(std::initializer_list<T> il);
    explicit deque();
    ~deque();
};

template <typename T, typename Alloc>
inline void deque<T, Alloc>::push_back(T to_push)
{
    
}

template <typename T, typename Alloc>
inline void deque<T, Alloc>::push_front(T to_push)
{
    
}

template <typename T, typename Alloc>
inline T deque<T, Alloc>::pop_back()
{
    return T();
}

template <typename T, typename Alloc>
inline T deque<T, Alloc>::pop_front()
{
    return T();
}

/*

Constructors, Destructor

*/

template <typename T, typename Alloc>
inline deque<T, Alloc>::deque(std::initializer_list<T> li)
{
    // allocate space for the list plus a little extra
    this->alloc = Alloc();
    this->_capacity = ((li.size * 1.5) < 4) ? (li.size * 2) : (li.size * 1.5);
    this->buffer = this->alloc.allocate(this->_capacity);
    
    // push our elements
    this->_size = 0;
    for (auto elem: li)
    {
        std::allocator_traits<Alloc>::construct(this->alloc, &this->buffer[this->_size], elem);
        this->_size += 1;
    }
}

template <typename T, typename Alloc>
inline deque<T, Alloc>::deque()
{
    this->alloc = Alloc();
    this->_capacity = 0;
    this->_size = 0;
    this->buffer = nullptr;
}

template <typename T, typename Alloc>
inline deque<T, Alloc>::~deque()
{
    // destroy any remaining objects in the deque
    for (size_t i = 0; i < this->_size; i++)
    {
        std::allocator_traits<Alloc>::destroy(this->alloc, &this->first_elem[i]);
    }

    // deallocate the space
    this->alloc.deallocate(this->buffer, this->_capacity);
    this->buffer = nullptr;
    this->_capacity = 0;
    this->_size = 0;
}
