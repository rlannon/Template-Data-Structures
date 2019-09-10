/*

Algorithms and Data Structures
Copyright 2019 Riley Lannon
Iterator.h

An implementation of a basic forward-iterator class

*/

#pragma once

template <typename T>
class forward_iterator
{
	T* ptr;
public:
	virtual forward_iterator<T>& operator++();
	virtual forward_iterator<T>& operator++(T);
	virtual T operator*();
	virtual bool operator==(const forward_iterator<T> right);
	virtual bool operator!=(const forward_iterator<T> right);

	forward_iterator(T* ptr);
	forward_iterator();
	virtual ~forward_iterator();
};

template <typename T>
forward_iterator<T>& forward_iterator<T>::operator++()
{
	ptr++;
	return *this;
}

template <typename T>
forward_iterator<T>& forward_iterator<T>::operator++(T)
{
	ptr++;
	return *this;
}

template <typename T>
T forward_iterator<T>::operator*()
{
	return *this->ptr;
}

template <typename T>
bool forward_iterator<T>::operator==(const forward_iterator<T> right)
{
	return this->ptr == right.ptr;
}

template <typename T>
bool forward_iterator<T>::operator!=(const forward_iterator<T> right)
{
	return this->ptr != right.ptr;
}

template <typename T>
forward_iterator<T>::forward_iterator(T* ptr)
{
	this->ptr = ptr;
}

template <typename T>
forward_iterator<T>::forward_iterator()
{
	this->ptr = nullptr;
}

template <typename T>
forward_iterator<T>::~forward_iterator()
{

}
