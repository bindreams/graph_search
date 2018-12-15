#pragma once
#include "node.hpp"
#include "util/pretty_print.hpp"

template<class T>
inline T& node<T>::value() {
	return *m_value;
}

template<class T>
inline const T& node<T>::value() const {
	return *m_value;
}

template<class T>
inline void node<T>::fw_connect(node* n) {
	m_edges.emplace(n);
}

template<class T>
inline void node<T>::bi_connect(node* n) {
	fw_connect(n);
	n->fw_connect(this);
}

template<class T>
inline void node<T>::fw_disconnect(node* n) {
	m_edges.erase(n);
}

template<class T>
inline void node<T>::bi_disconnect(node* n) {
	fw_disconnect(n);
	n->fw_disconnect(this);
}

template<class T>
inline std::size_t node<T>::id() const noexcept {
	return reinterpret_cast<std::size_t>(m_value.get());
}

template<class T>
template<class... Args>
inline node<T>::node(Args&&... args) :
	m_value(std::make_unique<T>(std::forward<Args>(args)...)) {
}

template<class T>
inline node<T>::~node() {
	for (auto& nd : adjacent_nodes()) {
		nd.m_edges.erase(this);
	}
}

template<class T>
inline typename node<T>::nodes_view 
node<T>::adjacent_nodes() noexcept {
	return nodes_view(
		node_iterator(m_edges.begin()), 
		node_iterator(m_edges.end()),
		const_node_iterator(m_edges.cbegin()),
		const_node_iterator(m_edges.cend()));
}

template<class T>
inline typename node<T>::const_nodes_view 
node<T>::adjacent_nodes() const noexcept {
	return const_nodes_view(
		const_node_iterator(m_edges.cbegin()), 
		const_node_iterator(m_edges.cend()));
}

template <class T>
std::ostream& operator<<(std::ostream& os, const node<T>& obj) {
	os << "{#" << pretty(obj.id()) << ": " << obj.value() << "}";
	return os;
}