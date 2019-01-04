#pragma once
#include "graph.hpp"

#include "deps/zh/container_view.hpp"

template <class T>
class graph<T>::const_nodes_view : public zh::const_forward_container_view<
	container,
	const_node_iterator> {
private:
	using base = zh::const_forward_container_view<
		container,
		const_node_iterator
	>;

public:
	using base::base;
};