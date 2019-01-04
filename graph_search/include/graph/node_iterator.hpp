#pragma once
#include "graph.hpp"

#include <iterator>
#include "deps/zh/proxy_iterator.hpp"

template <class T>
class graph<T>::node_iterator : public zh::proxy_iterator<
		container_iterator, zh::proxy::remove_const> {
	using base = zh::proxy_iterator<
		container_iterator, zh::proxy::remove_const>;
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
	node_iterator() = default;

	// From internal container
	explicit node_iterator(const container_iterator& other);

	// From a node::node_iterator
	node_iterator(const typename node_type::node_iterator& other);

	// From iterator
	explicit node_iterator(const iterator& other);
};

#include "node_iterator.inl"