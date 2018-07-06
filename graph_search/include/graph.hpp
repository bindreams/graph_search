#pragma once
#include <map>

#include "node.hpp"
#include "proxy_iterator.hpp"

#include "deps/json.hpp"
using json = nlohmann::json;

namespace detail {

struct get_second {
	template <class PairType>
	const typename PairType::second_type& 
		operator()(const PairType& p) const {
		return p.second;
	}

	template <class PairType>
	typename PairType::second_type& 
		operator()(PairType& p) const {
		return p.second;
	}
};

}

template <class T>
class graph {
private:
	using container = std::map<size_t, node<T>>;
	container nodes;

public:
	// Member types ============================================================
	using value_type      = T;
	using node_type       = node<T>;
	using size_type       = typename container::size_type;
	using difference_type = typename container::difference_type;
	using value_compare   = typename container::value_compare;
	using allocator_type  = typename container::value_compare;
	using reference       = typename container::reference;
	using const_reference = typename container::const_reference;
	using pointer         = typename container::pointer;
	using const_pointer   = typename container::const_pointer;

	using iterator = proxy_iterator<
		typename container::iterator, detail::get_second>;
	using const_iterator = proxy_iterator<
		typename container::const_iterator, detail::get_second>;
	using reverse_iterator = proxy_iterator<
		typename container::reverse_iterator, detail::get_second>;
	using const_reverse_iterator = proxy_iterator<
		typename container::const_reverse_iterator, detail::get_second>;

	// Member functions ========================================================
	// Constructors ------------------------------------------------------------
	constexpr graph() = default;
	graph(const graph& other);
	graph(graph&& other);
	~graph() = default;

	graph& operator=(graph rhs);

	template <class T_>
	friend inline void swap(graph<T_>& first, graph<T_>& second);

	// Iterators ---------------------------------------------------------------
	iterator begin() noexcept;
	const_iterator begin() const noexcept;
	const_iterator cbegin() const noexcept;

	iterator end() noexcept;
	const_iterator end() const noexcept;
	const_iterator cend() const noexcept;

	reverse_iterator rbegin() noexcept;
	const_reverse_iterator rbegin() const noexcept;
	const_reverse_iterator crbegin() const noexcept;

	reverse_iterator rend() noexcept;
	const_reverse_iterator rend() const noexcept;
	const_reverse_iterator crend() const noexcept;

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

	bool empty() const noexcept;
	void clear() noexcept;

	node<T>& operator[](size_t idx);
	const node<T>& operator[](size_t idx) const;

	size_t count_edges() const noexcept;
	double ratio() const noexcept;
	size_t size() const noexcept;

	template <class T_>
	friend void from_json(const json& j, graph<T_>& obj);
};

#include "inline/graph.inl"