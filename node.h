/*

Algorithms and Data Structures
Copyright 2019 Riley Lannon
node.h

A node class for a linked list

*/

#pragma once

#include <exception>

template<class T>
class list_node
{
	T data;
	list_node<T>* next;
public:
	T& get_data();
	list_node<T>* get_next();

	void set_data(T data);
	void set_next(list_node<T>* next);

	list_node(T data, list_node<T>* next = nullptr);
	list_node();
	~list_node();
};

template <typename T>
inline void list_node<T>::set_data(T data)
{
	this->data = data;
}

template <typename T>
inline T& list_node<T>::get_data()
{
	return this->data;
}

template <typename T>
inline list_node<T>* list_node<T>::get_next()
{
	return this->next;
}

template <typename T>
inline void list_node<T>::set_next(list_node<T>* next)
{
	this->next = next;
}

template <typename T>
list_node<T>::list_node(T data, list_node<T>* next)
{
	this->data = data;
	this->next = next;
}

template <typename T>
list_node<T>::list_node()
{
}

template <typename T>
list_node<T>::~list_node()
{
}
