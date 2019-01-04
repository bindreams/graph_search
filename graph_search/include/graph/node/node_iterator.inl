#pragma once
#include "node_iterator.hpp"

template <class T>
inline node<T>::node_iterator::node_iterator(
	const container_iterator& other,
	const node_lookup<T>& lookup)
	: base(other, lookup) {
}

template<class T>
inline bool node<T>::node_iterator::is_valid() const noexcept {
	return (*this) != base::get_functor().get_node().adjacent_nodes().end();
}

template<class T>
inline node<T>::node_iterator::operator bool() const noexcept {
	return is_valid();
}
