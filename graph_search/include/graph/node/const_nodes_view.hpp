#pragma once
#include "node.hpp"
#include "deps/zh/container_view.hpp"
#include "node_lookup.hpp"

template <class T>
class node<T>::const_nodes_view : public zh::const_forward_container_view<
	container,
	const_node_iterator,
	const_node_lookup<T>> {
private:
	using base = zh::const_forward_container_view<
		container,
		const_node_iterator,
		const_node_lookup<T>
	>;

public:
	using base::base;
};