/*

Algorithms and Data Structures
Copyright 2019 Riley Lannon
Queue.h

An implementation of the Queue data structures using C++ templates

*/

#pragma once

#include <vector>
#include <exception>

template <class T>
class Queue
{
	std::vector<T> data;
public:
	void push(T const& elem);

	T peek() const;
	T pop();

	bool is_empty() const;

	Queue();
	~Queue();
};

template <class T>
inline void Queue<T>::push(T const& elem)
{
	this->data.push_back(elem);
	return;
}

template <class T>
inline T Queue<T>::peek() const
{
	if (this->data.empty())
	{
		throw std::out_of_range("Queue<T> : cannot peek empty queue");
	}
	else
	{
		return this->data[0];
	}
}

template <class T>
inline T Queue<T>::pop()
{
	if (this->data.empty())
	{
		throw std::out_of_range("Queue<T> : cannot pop from empty queue");
	}
	else
	{
		T to_return = this->data[0];
		this->data.erase(this->data.begin());
		return to_return;
	}
}

template <class T>
inline bool Queue<T>::is_empty() const
{
	return this->data.empty();
}

template<class T> Queue<T>::Queue()
{
}

template<class T> Queue<T>::~Queue()
{
}
