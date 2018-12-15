#pragma once
#include <vector>
#include "node.hpp"
#include "deps/zh/proxy_iterator.hpp"
#include "deps/zh/container_view.hpp"
#include "functors/graph_functors.hpp"

#include "deps/json.hpp"
using json = nlohmann::json;

template <class T>
class graph {
private:
	using container = std::vector<node<T>>;
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

	using iterator
		= zh::proxy_iterator<typename container::iterator, access_value>;
	using const_iterator 
		= zh::proxy_iterator<typename container::const_iterator, access_value>;

	using node_iterator       = typename container::iterator;
	using const_node_iterator = typename container::const_iterator;

	// nodes_view is a container_view - a lightweight struct that provides a
	// way to access elements inside via begin and end. Since there are several
	// ways to iterate over a graph (values, nodes, edges), graph provides
	// nodes_view, edges_view, as well as plain begin/end for values.
	using nodes_view = zh::container_view<node_iterator, const_node_iterator>;
	using const_nodes_view = zh::const_container_view<const_node_iterator>;

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
	void connect(node_iterator it1, node_iterator it2);
	void disconnect(node_type& n1, node_type& n2);
	void disconnect(node_iterator it1, node_iterator it2);

	// Element access ----------------------------------------------------------
	
	// "Every container has to have a front() and back(). Except maybe 
	//  forward_list, but don't use forward_list"
	//     - andreasxp 2018
	reference front();
	const_reference front() const;
	reference back();
	const_reference back() const;

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