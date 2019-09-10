/*

Algorithms and Data Structures
Copyright 2019 Riley Lannon
Stack.h

An implementation of the Stack data structure using C++ templates

*/

#pragma once

#include <vector>
#include <exception>
#include <stdexcept>

template <typename T>
class stack
{
	std::vector<T> data;
public:
	void push(T const& elem);
	T pop();
	T peek() const;

	bool is_empty() const;

	stack();
	~stack();
};

template <typename T>
inline void stack<T>::push(T const& elem)
{
	this->data.push_back(elem);
	return;
}

template <typename T>
inline T stack<T>::pop()
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

template <typename T>
inline T stack<T>::peek() const
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

template <typename T>
inline bool stack<T>::is_empty() const
{
	return this->data.empty();
}

template <typename T>
inline stack<T>::stack()
{
}

template <typename T>
inline stack<T>::~stack()
{
}
