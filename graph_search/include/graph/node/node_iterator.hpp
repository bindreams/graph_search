#pragma once
#include "node.hpp"

#include <iterator>
#include "deps/zh/proxy_iterator.hpp"
#include "node_lookup.hpp"

template <class T>
class node<T>::node_iterator
	: public zh::proxy_iterator<container_iterator, node_lookup<T>> {
	using base = zh::proxy_iterator<container_iterator, node_lookup<T>>;
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
	node_iterator(
		const container_iterator& other,
		const node_lookup<T>& lookup);

	// Observers ==========================================================

	// Returns true if the iterator is safe to dereference.
	bool is_valid() const noexcept;
	explicit operator bool() const noexcept;
};

#include "node_iterator.inl"