#pragma once
#include <iostream>
#include "node_lookup.hpp"
#include "../graph.hpp"
#include "util/assert.hpp"

template <class T>
inline node<T>& 
node_lookup<T>::operator()(const typename node<T>::id_type& id) const noexcept {
	// This functor assumes that it will not be used to lookup nodes that do not exist.
	GS_ASSERT(get_graph().find(id) != get_graph().nodes().end());

	return *(get_graph().find(id));
}

template<class T>
inline node<T>& node_lookup<T>::get_node() const noexcept {
	return nd.get();
}

template<class T>
inline graph<T>& node_lookup<T>::get_graph() const noexcept {
	return nd.get().owner();
}

template<class T>
inline node_lookup<T>::node_lookup(node<T>& parent_node)
	: nd(parent_node) {
}

template <class T>
inline const node<T>& 
const_node_lookup<T>::operator()(typename node<T>::id_type id) const noexcept {
	// This functor assumes that it will not be used to lookup nodes that do not exist.
	GS_ASSERT(get_graph().find(id) != get_graph().nodes().end());

	return *(get_graph().find(id));
}

template<class T>
inline const node<T>& const_node_lookup<T>::get_node() const noexcept {
	return nd.get();
}

template<class T>
inline const graph<T>& const_node_lookup<T>::get_graph() const noexcept {
	return nd.get().owner();
}

template<class T>
inline const_node_lookup<T>::const_node_lookup(const node<T>& parent_node)
	: nd(parent_node) {
}

template<class T>
inline const_node_lookup<T>::const_node_lookup(const node_lookup<T>& other) 
	: nd(other.nd) {
}
