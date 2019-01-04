#pragma once
#include "const_node_iterator.hpp"
#include "iterator.hpp"
#include "const_iterator.hpp"
#include "node_iterator.hpp"

template <class T>
inline graph<T>::const_node_iterator::const_node_iterator(const node_iterator& other)
	: base(other) {
}

template <class T>
inline graph<T>::const_node_iterator::const_node_iterator(const const_container_iterator& other)
	: base(other) {
}

template <class T>
inline graph<T>::const_node_iterator::const_node_iterator(const typename node_type::node_iterator& other)
// other iterator was end() or default-constructed, default-construct
// otherwise, get node_iterator to the id stored in other.get_iterator()
	: const_node_iterator(other.is_valid() ?
		other.get_functor().get_graph().find(*other.get_iterator()) :
		other.get_functor().get_graph().nodes().cend()) {
}

template <class T>
inline graph<T>::const_node_iterator::const_node_iterator(const typename node_type::const_node_iterator& other)
// other iterator was end() or default-constructed, default-construct
// otherwise, get node_iterator to the id stored in other.get_iterator()
	: const_node_iterator(other.is_valid() ?
		other.get_functor().get_graph().find(*other.get_iterator()) :
		other.get_functor().get_graph().nodes().cend()) {
}

template <class T>
inline graph<T>::const_node_iterator::const_node_iterator(const iterator& other)
	: base(other.get_iterator()) {
}

template <class T>
inline graph<T>::const_node_iterator::const_node_iterator(const const_iterator& other)
	: base(other.get_iterator()) {
}
