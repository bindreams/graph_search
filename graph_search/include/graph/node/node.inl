#pragma once
#include "node.hpp"
#include "util/pretty_print.hpp"

template<class T>
inline bool node<T>::const_node_iterator::is_valid() const noexcept {
	return (*this) != base::get_functor().get_node().adjacent_nodes().end();
}

template<class T>
inline node<T>::const_node_iterator::operator bool() const noexcept {
	return is_valid();
}

template<class T>
inline T& node<T>::value() {
	return *m_value;
}

template<class T>
inline const T& node<T>::value() const {
	return *m_value;
}

template<class T>
inline void node<T>::fw_connect(const node& n) {
	m_edges.emplace(n.id());
}

template<class T>
inline void node<T>::bi_connect(node& n) {
	fw_connect(n);
	n.fw_connect(*this);
}

template<class T>
inline void node<T>::fw_disconnect(const node& n) {
	m_edges.erase(n.id());
}

template<class T>
inline void node<T>::bi_disconnect(node& n) {
	fw_disconnect(n);
	n.fw_disconnect(*this);
}

template<class T>
inline typename node<T>::id_type node<T>::id() const noexcept {
	return reinterpret_cast<id_type>(m_value.get());
}

template<class T>
inline graph<T>& node<T>::owner() noexcept {
	return m_owner;
}

template<class T>
inline const graph<T>& node<T>::owner() const noexcept {
	return m_owner;
}

template<class T>
inline node<T>::node(graph<T>& owner, T* adopt_ptr, container edges) :
	m_owner(owner),
	m_value(adopt_ptr),
	m_edges(edges) {
}

template<class T>
template<class... Args>
inline node<T>::node(graph<T>& owner, Args&&... args) :
	m_owner(owner),
	m_value(std::make_unique<T>(std::forward<Args>(args)...)) {
}

template<class T>
inline typename node<T>::nodes_view 
node<T>::adjacent_nodes() noexcept {
	return nodes_view(m_edges, *this);
}

template<class T>
inline typename node<T>::const_nodes_view 
node<T>::adjacent_nodes() const noexcept {
	return const_nodes_view(m_edges, *this);
}

template <class T>
std::ostream& operator<<(std::ostream& os, const node<T>& obj) {
	os << "{#" << pretty(obj.id()) << ": " << obj.value() << "}";
	return os;
}

namespace std {

template <class T>
struct hash<node<T>> {
	std::size_t operator()(const node<T>& nd) const noexcept {
		return std::hash<typename node<T>::id_type>()(nd.id());
	}
};

}