#pragma once
#include <functional>
#include "node.hpp"

template <class T>
class graph;

template <class T>
class node_lookup {
private:
	// node_lookup must remember the graph, in which it will
	// look up values. But this node_lookup also holds a pointer to the node
	// it was constructed from, which allows the iterator, constructed with it
	// to know if it reached end(). This is needed to convert
	// node<T>::node_iterator to graph<T>::node_iterator without dereferencing
	// it if it's at end().
	std::reference_wrapper<node<T>> nd;

public:
	// Constructor ------------------------------------------------------------
	explicit node_lookup(node<T>& parent_node);

	// operator() -------------------------------------------------------------
	node<T>& operator()(const typename node<T>::id_type& id) const noexcept;

	// Data access ------------------------------------------------------------
	node<T>&  get_node() const noexcept;
	graph<T>& get_graph() const noexcept;
};

template <class T>
class const_node_lookup {
private:
	std::reference_wrapper<const node<T>> nd;

public:
	// Constructors -----------------------------------------------------------
	explicit const_node_lookup(const node<T>& parent_node);
	
	const_node_lookup(const node_lookup<T>& other);

	// operator() -------------------------------------------------------------
	const node<T>& operator()(typename node<T>::id_type id) const noexcept;

	// Data access ------------------------------------------------------------
	const node<T>&  get_node() const noexcept;
	const graph<T>& get_graph() const noexcept;
};

#include "inline/node_lookup.inl"