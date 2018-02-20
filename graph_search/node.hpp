#pragma once
#include <vector>

template <class T>
class node {
	template <class T_>
	friend class graph_impl;
private:
	size_t id = 0;

public:
	T value;
	std::vector<node*> edges;

	size_t get_id() const;

	node(const T& value_, size_t id_ = 0);
	~node();
};

template<class T>
inline size_t node<T>::get_id() const {
	return id;
}

template<class T>
inline node<T>::node(const T& value_, size_t id_) :
	id (id_),
	value(value_) {
}

template<class T>
inline node<T>::~node() {
	for (auto&& connected_node : edges) {
		connected_node->edges.erase(
			std::find(connected_node->edges.begin(), connected_node->edges.end(), this));
	}
}

template<class T>
struct node_equal_values {
	inline bool operator() (const node<T>* const& lhs, const node<T>* const& rhs) const {
		return lhs->value == rhs->value;
	}
};

template<class T>
struct node_numerical_order {
	inline bool operator() (const node<T>* const& lhs, const node<T>* const& rhs) const {
		return lhs->value < rhs->value ||
			(lhs->value == rhs->value && lhs->get_id() < rhs->get_id());
	}
};

template <class T>
std::ostream& operator<<(std::ostream& os, const node<T>& obj) {
	os << "{#" << obj.get_id() << ": " << obj.value << "}";
	return os;
}