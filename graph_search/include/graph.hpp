#pragma once
#include <vector>
#include <unordered_set>
#include <memory>

#include "deps/ska/flat_hash_map.hpp"
#include "deps/zh/proxy_iterator.hpp"
#include "deps/zh/container_view.hpp"

#include "functors/graph_functors.hpp"
#include "functors/node_functors.hpp"

#include "node.hpp"

#include "deps/json.hpp"
using json = nlohmann::json;

template <class T>
class graph {
private:
	using container = ska::flat_hash_set<node<T>, std::hash<node<T>>, node_id_equal<T>>;
	
	using container_iterator = typename container::iterator;
	using const_container_iterator = typename container::const_iterator;

	container m_nodes;

public:
	// Member types ============================================================
	using value_type      = T;
	using node_type       = node<T>;
	using size_type       = typename container::size_type;
	using difference_type = typename container::difference_type;
	using allocator_type  = typename container::allocator_type;
	using reference       = typename container::reference;
	using const_reference = typename container::const_reference;
	using pointer         = typename container::pointer;
	using const_pointer   = typename container::const_pointer;

	// The followingg iterators and views are implemented as nested classes for
	// different reasons:
	// 1. More straightforward casting between iterators
	// 2. Can't forward-declare using directives
	// 3. Clearer error messages without using directives

	// Iterate over values
	class iterator;
	class const_iterator;

	// Iterate over nodes
	class node_iterator;
	class const_node_iterator;

	// nodes_view is a container_view - a lightweight struct that provides a
	// way to access elements inside via begin and end. Since there are several
	// ways to iterate over a graph (values, nodes, edges), graph provides
	// nodes_view, edges_view, as well as plain begin/end for values.
	class nodes_view;
	class const_nodes_view;

	class iterator : public zh::proxy_iterator<container_iterator, zh::proxy::remove_const, access_value> {
		using base = zh::proxy_iterator<container_iterator, zh::proxy::remove_const, access_value>;
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
		iterator() = default;

		// From internal container
		explicit iterator(const container_iterator& other);

		// From node_iterator
		explicit iterator(const node_iterator& other);
	};

	class const_iterator : public zh::proxy_iterator<const_container_iterator, access_value> {
		using base = zh::proxy_iterator<const_container_iterator, access_value>;
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
		const_iterator() = default;

		// Make const
		const_iterator(const iterator& other);

		// From internal container
		explicit const_iterator(const const_container_iterator& other);

		// From node_iterator
		explicit const_iterator(const node_iterator& other);
		explicit const_iterator(const const_node_iterator& other);
	};

	class node_iterator : public zh::proxy_iterator<container_iterator, zh::proxy::remove_const> {
		using base = zh::proxy_iterator<container_iterator, zh::proxy::remove_const>;
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
		explicit node_iterator(const container_iterator& other);

		// From a node::node_iterator
		node_iterator(const typename node_type::node_iterator& other);

		// From iterator
		explicit node_iterator(const iterator& other);
	};

	class const_node_iterator : public zh::make_deriveable<const_container_iterator> {
		using base = zh::make_deriveable<const_container_iterator>;
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
		explicit const_node_iterator(const const_container_iterator& other);

		// From a node::node_iterator
		const_node_iterator(const typename node<T>::node_iterator& other);
		const_node_iterator(const typename node_type::const_node_iterator& other);

		// From iterator
		explicit const_node_iterator(const iterator& other);
		explicit const_node_iterator(const const_iterator& other);
	};

	// nodes_view is a container_view - a lightweight struct that provides a
	// way to access elements inside via begin and end. Since there are several
	// ways to iterate over a graph (values, nodes, edges), graph provides
	// nodes_view, edges_view, as well as plain begin/end for values.
	class nodes_view : public zh::forward_container_view <
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

	class const_nodes_view : public zh::const_forward_container_view<
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

	// Member functions ========================================================
	// Constructors ------------------------------------------------------------
	constexpr graph() = default;
	graph(const graph& other);
	graph(graph&& other) noexcept;
	~graph() = default;

	graph& operator=(graph rhs);

	template <class T_>
	friend inline void swap(graph<T_>& first, graph<T_>& second) noexcept;

	// Iterators ---------------------------------------------------------------
	iterator begin() noexcept;
	const_iterator begin() const noexcept;
	const_iterator cbegin() const noexcept;

	iterator end() noexcept;
	const_iterator end() const noexcept;
	const_iterator cend() const noexcept;

	// Node iterators ----------------------------------------------------------
	nodes_view nodes() noexcept;
	const_nodes_view nodes() const noexcept;

	// Connecting --------------------------------------------------------------

	void connect(node_type& n1, node_type& n2);
	void connect(iterator it1, iterator it2);
	void connect(node_iterator it1, node_iterator it2);

	void disconnect(node_type& n1, node_type& n2);
	void disconnect(iterator it1, iterator it2);
	void disconnect(node_iterator it1, node_iterator it2);

	// Element access ----------------------------------------------------------
	
	// "Every container has to have a front() and back(). Except maybe 
	//  forward_list, but don't use forward_list"
	//     - andreasxp 2018
	reference front();
	const_reference front() const;
	reference back();
	const_reference back() const;

	// Lookup ------------------------------------------------------------------
	node_iterator       find(typename node<T>::id_type id);
	const_node_iterator find(typename node<T>::id_type id) const;

	// Modifiers ---------------------------------------------------------------
	iterator insert(const T& val);
	iterator insert(T&& val);

	template <class InputIt>
	void insert(InputIt first, InputIt last);
	void insert(std::initializer_list<value_type> ilist);

	template <class... Args>
	iterator emplace(Args&&... args);

	iterator erase(iterator it);
	node_iterator erase(node_iterator it);
	void clear() noexcept;

	void reserve(size_type new_size);

	// Observers --------------------------------------------------------------
	bool empty() const noexcept;
	std::size_t size() const noexcept;

	std::size_t count_edges() const noexcept;
	double ratio() const noexcept;
};

template <class T>
std::ostream& operator<<(std::ostream& os, const graph<T>& obj);

template <class T>
void to_json(json& j, const graph<T>& obj);

template <class T>
void from_json(const json& j, graph<T>& obj);

#include "inline/graph.inl"