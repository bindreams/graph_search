#pragma once
#include <unordered_set>
#include <memory>
#include <iomanip>
#include <cstdint>
#include "deps/zh/proxy_iterator.hpp"
#include "deps/zh/container_view.hpp"
#include "deps/zh/functor_sequence.hpp"
#include "functors/graph_functors.hpp"

#include "deps/json.hpp"
using json = nlohmann::json;

template <class T>
class node {
private:
	// Members -----------------------------------------------------------------
	std::unique_ptr<T> m_value;

	using container = std::unordered_set<node*>;
	container m_edges;

public:
	// Public types ============================================================

	// Warning: node iterators and node views are not the same as in class graph
	using node_iterator
		= zh::proxy_iterator<typename container::iterator,
		zh::dereference>;
	using const_node_iterator
		= zh::proxy_iterator<typename container::const_iterator,
		zh::functor_sequence<zh::dereference, zh::add_const>>;
	using nodes_view 
		= zh::container_view<node_iterator, const_node_iterator>;
	using const_nodes_view 
		= zh::const_container_view<const_node_iterator>;

	// Constructors ------------------------------------------------------------
	template <class... Args>
	node(Args&&... args);

	node() = delete;
	node(const node&) = delete;
	node(node&&) = default;

	node& operator=(const node&) = delete;
	node& operator=(node&&) = default;
	~node();

	// Iterators ---------------------------------------------------------------
	nodes_view adjacent_nodes() noexcept;
	const_nodes_view adjacent_nodes() const noexcept;

	// Observers ---------------------------------------------------------------
	std::size_t id() const noexcept;

	// Member access -----------------------------------------------------------
	T& value();
	const T& value() const;

	// Friends -----------------------------------------------------------------
	template <class T_>
	friend class graph;

private:
	// Connecting --------------------------------------------------------------
	void fw_connect(node* n); //one-way connecting
	void bi_connect(node* n); //two-way connecting

	void fw_disconnect(node* n);
	void bi_disconnect(node* n);
};

template <class T>
std::ostream& operator<<(std::ostream& os, const node<T>& obj);

#include "inline/node.inl"