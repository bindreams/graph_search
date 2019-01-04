#pragma once
#include <functional>
#include <memory>
#include <iomanip>
#include <cstdint>
#include <deps/ska/flat_hash_map.hpp>
#include "deps/zh/proxy_iterator.hpp"
#include "deps/zh/container_view.hpp"
#include "functors/graph_functors.hpp"
#include "functors/node_lookup.hpp"

#include "deps/json.hpp"
using json = nlohmann::json;

template <class T>
class graph;

template <class T>
class node {
public:
	// Public types ============================================================
	using id_type = std::size_t;

public:
	// Private members and types -----------------------------------------------
	using container = ska::flat_hash_set<id_type>;
	using container_iterator       = typename container::iterator;
	using const_container_iterator = typename container::const_iterator;

	// m_owner needs to be in a wrapper, as node must be swappable, and thus,
	// move-assignable.
	std::reference_wrapper<graph<T>> m_owner;
	std::unique_ptr<T> m_value;
	container m_edges;

	node(graph<T>& owner, T* adopt_ptr = nullptr, container edges = container());

public:
	// Public types (cont.) ====================================================

	class node_iterator;
	class const_node_iterator;
	class nodes_view;
	class const_nodes_view;

	// Warning: node iterators and node views are not the same as in class graph
	class node_iterator 
		: public zh::proxy_iterator<container_iterator, node_lookup<T>> {
		using base = zh::proxy_iterator<container_iterator, node_lookup<T>>;
	public:
		// Member types =======================================================
		using value_type
			= typename std::iterator_traits<base>::value_type;
		using difference_type
			= typename std::iterator_traits<base>::difference_type;
		using pointer
			= typename std::iterator_traits<base>::pointer;
		using reference
			= typename std::iterator_traits<base>::reference;
		using iterator_category
			= typename std::iterator_traits<base>::iterator_category;

		// Constructors =======================================================

		// Default
		node_iterator() = default;

		// From internal container
		node_iterator(
			const container_iterator& other, 
			const node_lookup<T>& lookup);

		// Observers ==========================================================

		// Returns true if the iterator is safe to dereference.
		bool is_valid() const noexcept;
		explicit operator bool() const noexcept;
	};

	class const_node_iterator 
		: public zh::proxy_iterator<const_container_iterator, const_node_lookup<T>> {
		using base = zh::proxy_iterator<const_container_iterator, const_node_lookup<T>>;
	public:
		// Member types =======================================================
		using value_type
			= typename std::iterator_traits<base>::value_type;
		using difference_type
			= typename std::iterator_traits<base>::difference_type;
		using pointer
			= typename std::iterator_traits<base>::pointer;
		using reference
			= typename std::iterator_traits<base>::reference;
		using iterator_category
			= typename std::iterator_traits<base>::iterator_category;

		// Constructors =======================================================

		// Default
		const_node_iterator() = default;

		// Make const
		const_node_iterator(const node_iterator& other);

		// From internal container
		const_node_iterator(
			const container_iterator& other, 
			const const_node_lookup<T>& lookup);
		const_node_iterator(
			const const_container_iterator& other, 
			const const_node_lookup<T>& lookup);

		// Observers ==========================================================

		// Returns true if the iterator is safe to dereference.
		bool is_valid() const noexcept;
		explicit operator bool() const noexcept;
	};

	class nodes_view : public zh::forward_container_view <
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

	class const_nodes_view : public zh::const_forward_container_view<
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

	// Constructors ------------------------------------------------------------
	template <class... Args>
	node(graph<T>& owner, Args&&... args);

	node() = delete;
	node(const node&) = delete;
	node(node&&) = default;

	node& operator=(const node&) = delete;
	node& operator=(node&&) = default;

	// Disconnecting edges is not the node's responsibility.
	// Graph does that when erasing the node.
	~node() = default;

	// Iterators ---------------------------------------------------------------
	nodes_view adjacent_nodes() noexcept;
	const_nodes_view adjacent_nodes() const noexcept;

	// Observers ---------------------------------------------------------------
	id_type id() const noexcept;

	graph<T>&       owner() noexcept;
	const graph<T>& owner() const noexcept;

	// Member access -----------------------------------------------------------
	T& value();
	const T& value() const;

	// Friends -----------------------------------------------------------------
	template <class T_>
	friend class graph;

	// see pseudonode.hpp
	template <class T_>
	friend class pseudonode;

private:
	// Connecting --------------------------------------------------------------
	void fw_connect(const node& n); //one-way connecting
	void bi_connect(node& n);       //two-way connecting

	void fw_disconnect(const node& n);
	void bi_disconnect(node& n);
};

template <class T>
std::ostream& operator<<(std::ostream& os, const node<T>& obj);

namespace std {

template <class T>
struct hash<node<T>>;

}

#include "inline/node.inl"