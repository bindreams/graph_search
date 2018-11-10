#pragma once
#include "deps/ska/unordered_map.hpp"
#include "graph.hpp"

template<class T>
inline graph<T>::graph(const graph& other) {
	// Reserve size for perfomance and to prevent iterator/pointer invalidation
	nodes.reserve(other.nodes.size());

	// Match other's ptr to node to equivalent ptr in this
	ska::unordered_map<const node<T>*, node<T>*> ptr_match;

	// Emplacing all nodes (not connecting)
	for (auto& nd : other) {
		nodes.emplace_back(nd.value());
		ptr_match[&nd] = &nodes.back();
	}

	// Connect all nodes by looking at other's edges and transforming pointers
	for (auto& nd : other) {
		for (auto& edge_ptr : nd.edges()) {
			connect(
				*ptr_match[&nd],
				*ptr_match[edge_ptr]
			);
		}
	}
}

template<class T>
inline graph<T>::graph(graph&& other) noexcept :
	graph() {
	swap(*this, other);
}

template<class T>
inline graph<T>& graph<T>::operator=(graph<T> other) {
	swap(*this, other);
	return *this;
}

template <class T_>
inline void swap(graph<T_>& lhs, graph<T_>& rhs) noexcept {
	// enable ADL
	using std::swap;

	swap(lhs.nodes, rhs.nodes);
}

// =============================================================================

template<class T>
inline typename graph<T>::iterator graph<T>::begin() noexcept {
	return nodes.begin();
}

template<class T>
inline typename graph<T>::const_iterator graph<T>::begin() const noexcept {
	return nodes.begin();
}

template<class T>
inline typename graph<T>::const_iterator graph<T>::cbegin() const noexcept {
	return nodes.cbegin();
}

template<class T>
inline typename graph<T>::iterator graph<T>::end() noexcept {
	return nodes.end();
}

template<class T>
inline typename graph<T>::const_iterator graph<T>::end() const noexcept {
	return nodes.end();
}

template<class T>
inline typename graph<T>::const_iterator graph<T>::cend() const noexcept {
	return nodes.cend();
}

template<class T>
inline void graph<T>::connect(node_type& n1, node_type& n2) {
	if (n1.id() == n2.id()) throw std::invalid_argument("graph::connect: cannot connect node to itself");
	n1.bi_connect(&n2);
}

template<class T>
inline void graph<T>::connect(iterator it1, iterator it2) {
	connect(*it1, *it2);
}

template<class T>
inline void graph<T>::disconnect(node_type& n1, node_type& n2) {
	n1.bi_disconnect(&n2);
}

template<class T>
inline void graph<T>::disconnect(iterator it1, iterator it2) {
	disconnect(*it1, *it2);
}

template<class T>
inline typename graph<T>::iterator graph<T>::insert(const T& val) {
	return nodes.emplace_back(val);
}

template<class T>
inline typename graph<T>::iterator graph<T>::insert(T&& val) {
	return nodes.emplace_back(std::move(val));
}

template<class T>
template<class InputIt>
void graph<T>::insert(InputIt first, InputIt last) {
	for (InputIt it = first; it != last; ++it) {
		insert(*it);
	}
}

template<class T>
inline void graph<T>::insert(std::initializer_list<value_type> ilist) {
	insert(ilist.begin(), ilist.end());
}

template<class T>
template<class... Args>
inline typename graph<T>::iterator graph<T>::emplace(Args&&... args) {
	nodes.emplace_back(std::forward<Args>(args)...);
	return std::prev(nodes.end());
}

template<class T>
inline typename graph<T>::iterator graph<T>::erase(iterator it) {
	return nodes.erase(it);
}

template<class T>
inline bool graph<T>::empty() const noexcept {
	return nodes.empty();
}

template<class T>
inline void graph<T>::clear() noexcept {
	nodes.clear();
}

template<class T>
inline void graph<T>::reserve(size_type new_size) {
	nodes.reserve(new_size);
}

template<class T>
inline std::size_t graph<T>::count_edges() const noexcept {
	std::size_t rslt = 0;

	for (auto&& i : nodes) {
		rslt += i.second.edges().size();
	}
	rslt /= 2;

	return rslt;
}

template<class T>
inline double graph<T>::ratio() const noexcept {
	std::size_t max_edges = size() * (size() - 1) / 2;
	std::size_t edges = count_edges();

	return static_cast<double>(edges) / max_edges;
}

template<class T>
inline std::size_t graph<T>::size() const noexcept {
	return nodes.size();
}

template <class T>
std::ostream& operator<<(std::ostream& os, const graph<T>& obj) {
	os << "{" << std::endl;

	for (auto& i : obj) {
		os << "    " << i;
		if (!i.edges().empty()) os << " -> ";

		for (auto& connection : i.edges()) {
			os << *connection << " ";
		}

		os << std::endl;
	}

	os << "}";

	return os;
}

template <class T>
void to_json(json& j, const graph<T>& obj) {
	for (auto&& nd : obj) {
		json temp;
		temp["value"] = nd.value();
		temp["id"] = nd.id();
		
		temp["edges"] = json::array();
		for (auto&& edge : nd.edges()) {
			temp["edges"].push_back(edge->id());
		}

		j.push_back(temp);
	}
}

template <class T>
void from_json(const json& j, graph<T>& obj) {
	// Clearing
	obj.clear();
	// Reserve size for perfomance and to prevent iterator/pointer invalidation
	obj.reserve(j.size());

	// Match other's id to node to equivalent ptr in this
	ska::unordered_map<std::size_t, node<T>*> ptr_match;

	// Emplacing all nodes (not connecting)
	for (auto& nd : j) {
		auto it = obj.emplace(nd["value"].get<T>());
		ptr_match[nd["id"]] = &*it;
	}

	// Connect all nodes by looking at j's ids and transforming pointers
	for (auto& nd : j) {
		for (auto& edge_id : nd["edges"]) {
			obj.connect(
				*ptr_match[nd["id"]],
				*ptr_match[edge_id]
			);
		}
	}
}