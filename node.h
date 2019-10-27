/*

Algorithms and Data Structures
Copyright 2019 Riley Lannon
node.h

A node class for a linked list

*/

#pragma once

#include <exception>

template<class T>
class node
{
	T data;
	node<T>* next;
public:
	// allow us to compare node<T> to T by checking the _data_ at the node with the righthand value
	bool operator==(T right);
	bool operator!=(T right);

	T& get_data();
	node<T>* get_next();

	void set_data(T data);
	void set_next(node<T>* next);

	node(T data, node<T>* next = nullptr);
	node();
	~node();
};

template <typename T>
inline bool node<T>::operator==(T right)
{
	return this->data == right;
}

template <typename T>
inline bool node<T>::operator!=(T right)
{
	return this->data != right;
}

template <typename T>
inline void node<T>::set_data(T data)
{
	this->data = data;
}

template <typename T>
inline T& node<T>::get_data()
{
	return this->data;
}

template <typename T>
inline node<T>* node<T>::get_next()
{
	return this->next;
}

template <typename T>
inline void node<T>::set_next(node<T>* next)
{
	this->next = next;
}

template <typename T>
node<T>::node(T data, node<T>* next)
{
	this->data = data;
	this->next = next;
}

template <typename T>
node<T>::node()
{
}

template <typename T>
node<T>::~node()
{
}
