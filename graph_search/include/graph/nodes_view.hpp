#pragma once
#include "graph.hpp"

#include "deps/zh/container_view.hpp"

template <class T>
class graph<T>::nodes_view : public zh::forward_container_view <
	container,
	node_iterator,
	const_node_iterator> {
private:
	using base = zh::forward_container_view <
		container,
		node_iterator,
		const_node_iterator
	>;

public:
	using base::base;
};