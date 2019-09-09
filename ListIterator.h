/*

Algorithms and Data Structures
Copyright 2019 Riley Lannon
ListIterator.h

The iterator class for our linked list, inheriting from MyIterator

*/

#pragma once

#include "Iterator.h"

template <class T>
class list_iterator : public forward_iterator<T>
{
	Node<T>* ptr;
public:
	list_iterator<T>& operator++();
	list_iterator<T>& operator++(T);
	T operator*();
	bool operator==(const list_iterator<T> right);
	bool operator!=(const list_iterator<T> right);
	bool operator==(const Node<T> right);
	bool operator!=(Node<T> right);

	Node<T>* get_ptr();

	list_iterator(Node<T>* ptr);
	list_iterator();
	~list_iterator();
};

template <class T>
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

template <class T>
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

template <class T>
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

template <class T>
bool list_iterator<T>::operator==(const list_iterator<T> right)
{
	return this->ptr == right.ptr;
}

template <class T>
bool list_iterator<T>::operator!=(const list_iterator<T> right)
{
	return this->ptr != right.ptr;
}

template <class T>
bool list_iterator<T>::operator==(const Node<T> right)
{
	return (this->ptr->get_data() == right.get_data()) && (this->ptr->get_next() == right.get_next());
}

template <class T>
bool list_iterator<T>::operator!=(Node<T> right)
{
	return !(this->ptr->get_data() == right.get_data()) && (this->ptr->get_next() == right.get_next());
}

template <class T>
Node<T>* list_iterator<T>::get_ptr()
{
	return this->ptr;
}

template <class T>
list_iterator<T>::list_iterator(Node<T>* ptr)
{
	this->ptr = ptr;
}

template <class T>
list_iterator<T>::list_iterator()
{

}

template <class T>
list_iterator<T>::~list_iterator()
{

}
