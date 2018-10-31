#pragma once
#include <map>
#include "graph.hpp"

template<class T>
inline graph<T>::graph(const graph& other) {
	std::map<size_t, size_t> match_id;

	// Adding all nodes
	for (auto&& nd : other) {
		node<T> temp(nd.value());
		size_t id = temp.id();
		nodes.emplace(id, std::move(temp));

		match_id[nd.id()] = id;
	}

	// Connecting
	for (auto&& nd : other) {
		for (auto&& connected_nd : nd.edges()) {
			connect(
				match_id[nd.id()], 
				match_id[connected_nd->id()]);
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
}

// =============================================================================

template<class T>
inline typename graph<T>::iterator graph<T>::begin() noexcept {
	return iterator(nodes.begin());
}

template<class T>
inline typename graph<T>::const_iterator graph<T>::begin() const noexcept {
	return const_iterator(nodes.begin());
}

template<class T>
inline typename graph<T>::const_iterator graph<T>::cbegin() const noexcept {
	return const_iterator(nodes.cbegin());
}

template<class T>
inline typename graph<T>::iterator graph<T>::end() noexcept {
	return iterator(nodes.end());
}

template<class T>
inline typename graph<T>::const_iterator graph<T>::end() const noexcept {
	return const_iterator(nodes.end());
}

template<class T>
inline typename graph<T>::const_iterator graph<T>::cend() const noexcept {
	return const_iterator(nodes.cend());
}

template<class T>
inline typename graph<T>::reverse_iterator graph<T>::rbegin() noexcept {
	return reverse_iterator(nodes.rbegin());
}

template<class T>
inline typename graph<T>::const_reverse_iterator graph<T>::rbegin() const noexcept {
	return const_reverse_iterator(nodes.rbegin());
}

template<class T>
inline typename graph<T>::const_reverse_iterator graph<T>::crbegin() const noexcept {
	return const_reverse_iterator(nodes.crbegin());
}

template<class T>
inline typename graph<T>::reverse_iterator graph<T>::rend() noexcept {
	return reverse_iterator(nodes.rend());
}

template<class T>
inline typename graph<T>::const_reverse_iterator graph<T>::rend() const noexcept {
	return const_reverse_iterator(nodes.rend());
}

template<class T>
inline typename graph<T>::const_reverse_iterator graph<T>::crend() const noexcept {
	return const_reverse_iterator(nodes.crend());
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
	node<T> temp(val);
	size_t id = temp.id();
	auto x = nodes.emplace(id, std::move(temp));

	return iterator(x.first);
}

template<class T>
inline typename graph<T>::iterator graph<T>::insert(T&& val) {
	node<T> temp(std::move(val));
	size_t id = temp.id();
	auto x = nodes.emplace(id, std::move(temp));

	return iterator(x.first);
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
	node<T> temp(std::forward<Args>(args)...);
	size_t id = temp.id();
	auto x = nodes.emplace(id, std::move(temp));

	return x.first;
}

template<class T>
inline typename graph<T>::iterator graph<T>::erase(iterator it) {
	return iterator(nodes.erase(it));
}

template<class T>
inline bool graph<T>::empty() const noexcept {
	return size() == 0;
}

template<class T>
inline void graph<T>::clear() noexcept {
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
inline size_t graph<T>::count_edges() const noexcept {
	size_t rslt = 0;

	for (auto&& i : nodes) {
		rslt += i.second.edges().size();
	}
	rslt /= 2;

	return rslt;
}

template<class T>
inline double graph<T>::ratio() const noexcept {
	size_t max_edges = size() * (size() - 1) / 2;
	size_t edges = count_edges();

	return static_cast<double>(edges) / max_edges;
}

template<class T>
inline size_t graph<T>::size() const noexcept {
	return nodes.size();
}

template <class T>
std::ostream& operator<<(std::ostream& os, const graph<T>& obj) {
	os << "{" << std::endl;

	for (auto&& i : obj) {
		os << "    " << i;
		if (!i.edges().empty()) os << " -> ";

		for (auto&& connection : i.edges()) {
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

	std::map<size_t, size_t> match_id;
	// Adding all nodes
	for (auto&& i : j) {
		size_t id = obj.insert(i["value"].get<T>())->id();

		match_id[i["id"]] = id;
	}

	// Connecting
	for (auto&& nd : j) {
		for (auto&& connected_id : nd["edges"]) {
			obj.connect(
				obj[match_id[nd["id"]]],
				obj[match_id[connected_id]]);
		}
	}
}