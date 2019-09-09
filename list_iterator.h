/*

Algorithms and Data Structures
Copyright 2019 Riley Lannon
ListIterator.h

The iterator class for our linked list, inheriting from MyIterator

*/

#pragma once

#include "iterator.h"

template <typename T>
class list_iterator : public forward_iterator<T>
{
	node<T>* ptr;
public:
	list_iterator<T>& operator++();
	list_iterator<T>& operator++(T);
	T operator*();
	bool operator==(const list_iterator<T> right);
	bool operator!=(const list_iterator<T> right);
	bool operator==(const node<T> right);
	bool operator!=(node<T> right);

	node<T>* get_ptr();

	list_iterator(node<T>* ptr);
	list_iterator();
	~list_iterator();
};

template <typename T>
list_iterator<T>& list_iterator<T>::operator++()
{
	if (this->ptr == nullptr)
	{
		throw std::out_of_range("Cannot advance iterator");
	}
	else {
		this->ptr = this->ptr->get_next();
		return *this;
	}
}

template <typename T>
list_iterator<T>& list_iterator<T>::operator++(T)
{
	if (this->ptr == nullptr)
	{
		throw std::out_of_range("Cannot advance iterator");
	}
	else {
		this->ptr = this->ptr->get_next();
		return *this;
	}
}

template <typename T>
T list_iterator<T>::operator*()
{
	if (this->ptr == nullptr)
	{
		throw std::out_of_range("Cannot dereference iterator");
	}
	else {
		return this->ptr->get_data();
	}
}

template <typename T>
bool list_iterator<T>::operator==(const list_iterator<T> right)
{
	return this->ptr == right.ptr;
}

template <typename T>
bool list_iterator<T>::operator!=(const list_iterator<T> right)
{
	return this->ptr != right.ptr;
}

template <typename T>
bool list_iterator<T>::operator==(const node<T> right)
{
	return (this->ptr->get_data() == right.get_data()) && (this->ptr->get_next() == right.get_next());
}

template <typename T>
bool list_iterator<T>::operator!=(node<T> right)
{
	return !(this->ptr->get_data() == right.get_data()) && (this->ptr->get_next() == right.get_next());
}

template <typename T>
node<T>* list_iterator<T>::get_ptr()
{
	return this->ptr;
}

template <typename T>
list_iterator<T>::list_iterator(node<T>* ptr)
{
	this->ptr = ptr;
}

template <typename T>
list_iterator<T>::list_iterator()
{

}

template <typename T>
list_iterator<T>::~list_iterator()
{

}
