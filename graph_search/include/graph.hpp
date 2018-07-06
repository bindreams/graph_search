#pragma once
#include <map>

#include "id_manager.hpp"
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
	id_manager ids;

public:
	// Member types ============================================================
	using value_type      = node<T>;
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

	//Member functions
	iterator begin();
	const_iterator begin() const;
	const_iterator cbegin() const;

	iterator end();
	const_iterator end() const;
	const_iterator cend() const;

	reverse_iterator rbegin();
	const_reverse_iterator rbegin() const;
	const_reverse_iterator crbegin() const;

	reverse_iterator rend();
	const_reverse_iterator rend() const;
	const_reverse_iterator crend() const;

	void connect(size_t n1, size_t n2);
	void disconnect(size_t n1, size_t n2);

	template <class InputIt>
	size_t push(const T& val, InputIt first, InputIt last);
	size_t push(const T& val, std::initializer_list<size_t> connect_to = {});
	void pop(size_t id);

	bool empty() const;
	void clear();

	node<T>& operator[](size_t idx);
	const node<T>& operator[](size_t idx) const;

	size_t count_edges() const;
	double ratio() const;
	size_t size() const;

	template <class T_>
	friend void from_json(const json& j, graph<T_>& obj);
};

// =============================================================================

template<class T>
inline graph<T>::graph(const graph& other) :
	ids(other.ids) {
	//Clearing
	clear();

	//Adding all nodes
	for (auto&& nd : other) {
		nodes.emplace(nd.get_id(), node<T>(*nd, nd.get_id()));
	}

	//Connecting
	for (auto&& nd : other) {
		for (auto&& connected_nd : nd.get_edges()) {
			connect(nd.get_id(), connected_nd->get_id());
		}
	}
}

template<class T>
inline graph<T>::graph(graph && other) :
	graph() {
	swap(*this, other);
}

template<class T>
inline graph<T>& graph<T>::operator=(graph<T> other) {
	swap(*this, other);
	return *this;
}

template <class T_>
inline void swap(graph<T_>& lhs, graph<T_>& rhs) {
	// enable ADL
	using std::swap;

	swap(lhs.nodes, rhs.nodes);
	swap(lhs.ids, rhs.ids);
}

// =============================================================================

template<class T>
inline typename graph<T>::iterator graph<T>::begin() {
	return nodes.begin();
}

template<class T>
inline typename graph<T>::const_iterator graph<T>::begin() const {
	return nodes.begin();
}

template<class T>
inline typename graph<T>::const_iterator graph<T>::cbegin() const {
	return nodes.cbegin();
}

template<class T>
inline typename graph<T>::iterator graph<T>::end() {
	return nodes.end();
}

template<class T>
inline typename graph<T>::const_iterator graph<T>::end() const {
	return nodes.end();
}

template<class T>
inline typename graph<T>::const_iterator graph<T>::cend() const {
	return nodes.cend();
}

template<class T>
inline typename graph<T>::reverse_iterator graph<T>::rbegin() {
	return nodes.rbegin();
}

template<class T>
inline typename graph<T>::const_reverse_iterator graph<T>::rbegin() const {
	return nodes.rbegin();
}

template<class T>
inline typename graph<T>::const_reverse_iterator graph<T>::crbegin() const {
	return nodes.crbegin();
}

template<class T>
inline typename graph<T>::reverse_iterator graph<T>::rend() {
	return nodes.rend();
}

template<class T>
inline typename graph<T>::const_reverse_iterator graph<T>::rend() const {
	return nodes.rend();
}

template<class T>
inline typename graph<T>::const_reverse_iterator graph<T>::crend() const {
	return nodes.crend();
}

template<class T>
inline void graph<T>::connect(size_t n1, size_t n2) {
	if (n1 == n2) throw std::invalid_argument("graph::connect: cannot connect node to itself");

	nodes.at(n1).bi_connect(&nodes.at(n2));
}

template<class T>
inline void graph<T>::disconnect(size_t n1, size_t n2) {
	nodes.at(n1).bi_disconnect(&nodes.at(n2));
}

template<class T>
template<class InputIt>
inline size_t graph<T>::push(const T& val, InputIt first, InputIt last) {
	size_t id = ids.get();
	node<T>& element = nodes.emplace(id, node<T>(val, id)).first->second;

	//Establish connections (edges)
	for (InputIt it = first; it != last; ++it) {
		element.bi_connect(&nodes.at(*it));
	}

	return id;
}

template<class T>
inline size_t graph<T>::push(const T& val, std::initializer_list<size_t> connect_to) {
	return push(val, connect_to.begin(), connect_to.end());
}

template<class T>
inline void graph<T>::pop(size_t id) {
	nodes.erase(id);
}

template<class T>
inline bool graph<T>::empty() const {
	return size() == 0;
}

template<class T>
inline void graph<T>::clear() {
	nodes.clear();
}

template<class T>
inline node<T>& graph<T>::operator[](size_t idx) {
	auto got = nodes.find(idx);
	if (got == nodes.end()) throw std::invalid_argument("graph::operator[]: node with this index does not exist");

	return got->second;
}

template<class T>
inline const node<T>& graph<T>::operator[](size_t idx) const {
	auto got = nodes.find(idx);
	if (got == nodes.end()) throw std::invalid_argument("graph::operator[]: node with this index does not exist");

	return got->second;
}

template<class T>
inline size_t graph<T>::count_edges() const {
	size_t rslt = 0;

	for (auto&& i : nodes) {
		rslt += i.second.get_edges().size();
	}
	rslt /= 2;

	return rslt;
}

template<class T>
inline double graph<T>::ratio() const {
	size_t max_edges = size() * (size() - 1) / 2;
	size_t edges = g.count_edges();

	return static_cast<double>(edges) / max_edges;
}

template<class T>
inline size_t graph<T>::size() const {
	return nodes.size();
}

template <class T>
std::ostream& operator<<(std::ostream& os, const graph<T>& obj) {
	os << "{" << std::endl;

	for (auto&& i : obj) {
		os << "    " << i;
		if (!i.get_edges().empty()) os << " -> ";

		for (auto&& connection : i.get_edges()) {
			os << *connection << " ";
		}

		os << std::endl;
	}

	os << "}";

	return os;
}

template <class T>
void to_json(json& j, const graph<T>& obj) {
	for (auto&& i : obj) {
		j["nodes"].push_back(i);
	}
}

template <class T>
void from_json(const json& j, graph<T>& obj) {
	//Clearing
	obj.clear();

	//Adding all nodes
	size_t max_id = 0;
	for (auto&& i : j["nodes"]) {
		node<T> n = i;

		size_t id = n.get_id();
		if (id > max_id) max_id = id;

		obj.nodes.emplace(id, std::move(n));
	}

	//Setting up id_manager
	obj.ids.set_next(max_id + 1);

	//Connecting
	for (auto&& nd : j["nodes"]) {
		for (auto&& connected_nd : nd["edges"]) {
			obj.connect(nd["id"], connected_nd);
		}
	}
}