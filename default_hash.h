#pragma once

#include <math.h>

const unsigned int mod_const = (int)pow(2, 32);
const size_t knuth_const = 2654435761;

template <typename K>
class default_hash
{
public:
	size_t operator()(const K &key) const;

	default_hash();
	~default_hash();
};

template <typename K>
size_t default_hash<K>::operator()(const K &key) const
{
	return (key * knuth_const) % mod_const;
}

template <>
size_t default_hash<float>::operator()(const float &key) const
{
	return 0;
}

template<>
size_t default_hash<std::string>::operator()(const std::string &key) const
{
	return key.length();
}

template <typename K>
default_hash<K>::default_hash()
{

}

template <typename K>
default_hash<K>::~default_hash()
{

}
