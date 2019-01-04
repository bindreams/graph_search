#pragma once
#include "const_iterator.hpp"
#include "iterator.hpp"
#include "node_iterator.hpp"
#include "const_node_iterator.hpp"

template <class T>
inline graph<T>::const_iterator::const_iterator(const iterator& other)
	: base(other) {
}

template <class T>
inline graph<T>::const_iterator::const_iterator(const const_container_iterator& other)
	: base(other) {
}

template <class T>
inline graph<T>::const_iterator::const_iterator(const node_iterator& other)
	: base(other.get_iterator()) {
}

template <class T>
inline graph<T>::const_iterator::const_iterator(const const_node_iterator& other)
	: base(other.get_iterator()) {
}