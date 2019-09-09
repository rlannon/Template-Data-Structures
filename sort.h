/*

Algorithms and Data Structures
Copyright 2019 Riley Lannon
sort.h

An implementation of a few sorting algorithms using C++ templates. Currently supported algorithms:
	- bubble sort
	- selection sort
	- insertion sort
	- merge sort
Each algorithm requires the use of the in/equality operators with respect to their types.
Further, each algorithm operates on vectors, and all use void functions that pass by reference.

*/

#pragma once

#include <vector>
#include <algorithm>

using std::vector;

template <typename T>
void bubble_sort(vector<T>& to_sort)
{
	/*
	
	Sorts a vector containing type T using the bubble sort algorithm.
	The only requirement for <typename T> is that the in/equality operators are defined.
	
	*/

	size_t num_swaps;	// to track how many swaps we have made; when the list is sorted, passing over it will mean we didn't perform any swaps

	// we want to iterate over the list at least once
	do
	{
		// reset the swap counter
		num_swaps = 0;

		// start i at one so we can compare i and [i - 1] without error
		for (size_t i = 1; i < to_sort.size(); i++)
		{
			if (to_sort[i] < to_sort[i - 1])
			{
				std::swap(to_sort[i], to_sort[i - 1]);
				num_swaps += 1;
			}
		}
	} while (num_swaps > 0);

	return;
}

template <typename T>
void selection_sort(vector<T>& to_sort)
{
	/*
	
	Implementation of selection sort on a vector of any type for which the in/equality operators are defined.

	*/

	size_t i, j;
	for (i = 0; i < to_sort.size(); i++)
	{
		// find the smallest element in unsorted portion of list (to_sort[i] .. to_sort[size - 1])
		// start with the first one, compare rest of list

		size_t min = i;		// track the /index/ of the smallest value

		// iterate through rest of list to find smallest
		for (j = i + 1; j < to_sort.size(); j++)
		{
			// if the value at j is less than the value at min,
			if (to_sort[j] < to_sort[min])
			{
				// update our index
				min = j;
			}
		}

		// finally, if we found a smaller value in the list, swap them (min and i will not be the same)
		if (min != i)
		{
			std::swap(to_sort[i], to_sort[min]);
		}
	}
	
	return;
}

template <typename T>
void insertion_sort(vector<T>& to_sort)
{
	/*
	
	Performs an insertion sort
	
	*/

	// begin at i = 1; first entry is skipped
	for (size_t i = 1; i < to_sort.size(); i++)
	{
		// iterate through the previous
		size_t j = 0;
		while ((j < i) && (to_sort[j] < to_sort[i]))
		{
			j++;
		}

		// delete the element at position i from the vector, then insert it at position j
		// since j < i, this won't cause any issues
		T temp = to_sort[i];
		to_sort.erase(to_sort.begin() + i);
		to_sort.insert(to_sort.begin() + j, temp);
	}

	return;
}

template <typename T>
void merge_sort(vector<T>& to_sort)
{
	/*
	
	A recursive merge sort implementation.

	*/
	
	if (to_sort.size() > 1)
	{
		size_t mid = to_sort.size() / 2;
		
		// create the two halves of our vector
		vector<T> left;
		for (size_t i = 0; i < mid; i++)
		{
			left.push_back(to_sort[i]);
		}

		vector<T> right;
		for (size_t i = mid; i < to_sort.size(); i++)
		{
			right.push_back(to_sort[i]);
		}

		// sort those halves
		merge_sort(left);
		merge_sort(right);

		// merge them
		size_t left_iter = 0, right_iter = 0;
		vector<T> merged;

		while (left_iter < left.size() && right_iter < right.size())
		{
			if (left[left_iter] < right[right_iter])
			{
				merged.push_back(left[left_iter]);
				left_iter++;
			}
			else
			{
				merged.push_back(right[right_iter]);
				right_iter++;
			}
		}

		// check to see if we have any elements left in either vector; only one of these loops will execute each time
		while (left_iter < left.size())
		{
			merged.push_back(left[left_iter]);
			left_iter++;
		}
		while (right_iter < right.size())
		{
			merged.push_back(right[right_iter]);
			right_iter++;
		}

		// now, merged contains the sorted elements
		to_sort = merged;
		return;
	}
	else
	{
		return;
	}
}
