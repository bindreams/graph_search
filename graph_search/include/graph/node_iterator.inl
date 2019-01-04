#pragma once
#include "node_iterator.hpp"
#include "iterator.hpp"

template <class T>
inline graph<T>::node_iterator::node_iterator(const container_iterator& other)
	: base(other) {
}

template <class T>
inline graph<T>::node_iterator::node_iterator(const typename node_type::node_iterator& other)
// other iterator was end() or default-constructed, default-construct
// otherwise, get node_iterator to the id stored in other.get_iterator()
	: node_iterator(other.is_valid() ?
		other.get_functor().get_graph().find(*other.get_iterator()) :
		other.get_functor().get_graph().nodes().end()) {
}

template <class T>
inline graph<T>::node_iterator::node_iterator(const iterator& other)
	: base(other.get_iterator()) {
}