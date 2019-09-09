/*

Algorithms and Data Structures
Copyright 2019 Riley Lannon
Node.h

A node class for a linked list

*/

#pragma once

#include <exception>

template<class T>
class Node
{
	T data;
	Node<T>* next;
public:
	T get_data() const;
	Node<T>* get_next();

	void set_data(T data);
	void set_next(Node<T>* next);

	Node(T data, Node<T>* next = nullptr);
	Node();
	~Node();
};

template <class T>
inline void Node<T>::set_data(T data)
{
	this->data = data;
}

template <class T>
inline T Node<T>::get_data() const
{
	return this->data;
}

template <class T>
inline Node<T>* Node<T>::get_next()
{
	return this->next;
}

template <class T>
inline void Node<T>::set_next(Node<T>* next)
{
	this->next = next;
}

template <class T>
Node<T>::Node(T data, Node<T>* next)
{
	this->data = data;
	this->next = next;
}

template <class T>
Node<T>::Node()
{
}

template <class T>
Node<T>::~Node()
{
}
