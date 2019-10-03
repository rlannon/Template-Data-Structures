/*

Algorithms and Data Structures
Copyright 2019 Riley Lannon
search.h

Implementations of various search algorithms using C++ templates

*/

#pragma once

#include <iterator>

template <typename Container> typename Container::iterator
linear_search(Container &target, typename Container::value_type const& to_find)
{
	/*
	linear_search
	Performs a linear search on any STL container, or any custom container for which a forward iterator and value_type are available
	The container does not need to be sorted

	@param	target	The container to search
	@param	to_find	The value we want to find; must be the same type as the Container holds

	@return	An iterator to the found member; if the value is not found, returns an iterator to the past-the-end element

	*/
	
	bool found = false;

	typename Container::iterator it = target.begin();
	while (it != target.end() && !found)
	{
		if (*it == to_find)
		{
			found = true;
		}
		else
		{
			it++;
		}
	}

	return it;
}

template <typename Container> typename Container::iterator
binary_search(Container &target, typename Container::value_type const& to_find, std::random_access_iterator_tag)
{
	/*
	
	binary_search()
	Performs a binary search on STL container, returning an iterator to the position of the element to be found if it can be located

	@param	target	The container to search
	@param	to_find	The data to search for in the container

	@return	An iterator to the element, if found; otherwise, it returns a past-the-end iterator

	*/

	size_t midway = target.size() / 2;
	typename Container::iterator l = target.begin();
	typename Container::iterator r = target.end();
	typename Container::iterator m = l + midway;
	midway /= 2;
	
	while ((l <= r) && (m != target.end()))
	{
		if (*m == to_find)
		{
			return m;
		}
		else if (*m > to_find)
		{
			r = m - 1;
		}
		else
		{
			l = m + 1;
		}

		m = l + midway;
		midway /= 2;
	}

	return target.end();
}

template <typename Container>
typename Container::iterator binary_search(Container &target, typename Container::value_type const& to_find)
{
	/*
	
	binary_search()
	Ensures that the container passed to the function is a random-access iterator
	
	*/
	
	return binary_search(target, to_find, std::iterator_traits<typename Container::iterator>::iterator_category());
}
