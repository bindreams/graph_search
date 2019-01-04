#pragma once
#include "node.hpp"
#include "deps/zh/container_view.hpp"
#include "node_lookup.hpp"

template <class T>
class node<T>::nodes_view : public zh::forward_container_view <
	container,
	node_iterator,
	const_node_iterator,
	node_lookup<T>> {
private:
	using base = zh::forward_container_view <
		container,
		node_iterator,
		const_node_iterator,
		node_lookup<T>
	>;

public:
	using base::base;
};