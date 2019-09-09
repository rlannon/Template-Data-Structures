/*

Algorithms and Data Structures
Copyright 2019 Riley Lannon
Stack.h

An implementation of the Stack data structure using C++ templates

*/

#pragma once

#include <vector>
#include <exception>

template <class T>
class Stack
{
	std::vector<T> data;
public:
	void push(T const& elem);
	T pop();
	T peek() const;

	bool is_empty() const;

	Stack();
	~Stack();
};

template <class T>
inline void Stack<T>::push(T const& elem)
{
	this->data.push_back(elem);
	return;
}

template <class T>
inline T Stack<T>::pop()
{
	if (this->data.size() == 0)
	{
		throw std::out_of_range("Stack<T>: cannot pop from empty stack");
	}
	else {
		T to_return = this->data.back();
		this->data.pop_back();
		return to_return;
	}
}

template <class T>
inline T Stack<T>::peek() const
{
	if (this->data.size() == 0)
	{
		throw std::out_of_range("Stack<T>: cannot peek empty stack");
	}
	else
	{
		return data.back();
	}
}

template <class T>
inline bool Stack<T>::is_empty() const
{
	return this->data.empty();
}

template <class T>
inline Stack<T>::Stack()
{
}

template <class T>
inline Stack<T>::~Stack()
{
}
