#pragma once
#include <unordered_set>
#include <memory>
#include <iomanip>
#include <cstdint>

#include "deps/json.hpp"
using json = nlohmann::json;

template <class T>
class node {
public:
	// Constructors ============================================================
	template<class... Args>
	node(Args... args);

	node() = delete;
	node(const node&) = delete;
	node(node&&) = default;

	node& operator=(const node&) = delete;
	node& operator=(node&&) = default;
	~node();

	// Observers ===============================================================
	std::size_t id() const;

	// Member access ===========================================================
	T& value();
	const T& value() const;

	const std::unordered_set<node*>& edges() const;

	// Friends =================================================================
	template <class T_>
	friend class graph;
private:
	// Connecting ==============================================================
	void fw_connect(node* n); //one-way connecting
	void bi_connect(node* n); //two-way connecting

	void fw_disconnect(node* n);
	void bi_disconnect(node* n);

	// Members =================================================================
	std::unique_ptr<T> m_value;
	std::unordered_set<node*> m_edges;
};

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
inline const std::unordered_set<node<T>*>& node<T>::edges() const {
	return m_edges;
}

template<class T>
inline std::size_t node<T>::id() const {
	return reinterpret_cast<std::size_t>(m_value.get());
}

template<class T>
template<class... Args>
inline node<T>::node(Args... args) :
	m_value(std::make_unique<T>(args...)) {
}

template<class T>
inline node<T>::~node() {
	for (auto&& connected_node : edges()) {
		connected_node->m_edges.erase(this);
	}
}

template<class T>
struct node_value_equal {
	inline bool operator() (const node<T>* const& lhs, const node<T>* const& rhs) const {
		return lhs->value() == rhs->value();
	}
};

template<class T>
struct node_value_order {
	inline bool operator() (const node<T>* const& lhs, const node<T>* const& rhs) const {
		return lhs->value() < rhs->value() ||
			(lhs->value() == rhs->value() && lhs->id() < rhs->id());
	}
};

template <class T>
std::ostream& operator<<(std::ostream& os, const node<T>& obj) {
	os << "{#" << std::hex << std::setfill('0') << std::setw(2) << obj.id()%0x1000 << ": " << obj.value() << "}";
	return os;
}