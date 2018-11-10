#pragma once
#include <vector>
#include "node.hpp"

#include "deps/json.hpp"
using json = nlohmann::json;

template <class T>
class graph {
private:
	using container = std::vector<node<T>>;
	container nodes;

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

	using iterator        = typename container::iterator;
	using const_iterator  = typename container::const_iterator;

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

	// Connecting --------------------------------------------------------------
	void connect(node_type& n1, node_type& n2);
	void connect(iterator it1, iterator it2);
	void disconnect(node_type& n1, node_type& n2);
	void disconnect(iterator it1, iterator it2);

	// Modifiers ---------------------------------------------------------------
	iterator insert(const T& val);
	iterator insert(T&& val);

	template <class InputIt>
	void insert(InputIt first, InputIt last);
	void insert(std::initializer_list<value_type> ilist);

	template <class... Args>
	iterator emplace(Args&&... args);

	iterator erase(iterator it);
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