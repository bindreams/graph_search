#pragma once
#include "graph.hpp"

#include <iterator>
#include "deps/zh/proxy_iterator.hpp"
#include "node/node_functors.hpp"

template <class T>
class graph<T>::iterator : public zh::proxy_iterator<
		container_iterator, zh::proxy::remove_const, access_value> {
	using base = zh::proxy_iterator<
		container_iterator, zh::proxy::remove_const, access_value>;
public:
	// Member types =======================================================
	using value_type
		= typename std::iterator_traits<base>::value_type;
	using difference_type
		= typename std::iterator_traits<base>::difference_type;
	using pointer
		= typename std::iterator_traits<base>::pointer;
	using reference
		= typename std::iterator_traits<base>::reference;
	using iterator_category
		= typename std::iterator_traits<base>::iterator_category;

	// Constructors =======================================================

	// Default
	iterator() = default;

	// From internal container
	explicit iterator(const container_iterator& other);

	// From node_iterator
	explicit iterator(const node_iterator& other);
};

#include "iterator.inl"