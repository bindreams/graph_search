#pragma once
#include <vector>
#include "print.hpp"

template <class T>
class node {
	template <class T_>
	friend class graph_impl;
public:
	T value;
	std::vector<node*> edges;

	node(const T& value_);
	~node();

	template <class T_>
	friend std::ostream& operator<<(std::ostream& os, const node<T_>& obj);
};

template<class T>
inline node<T>::node(const T & value_) :
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
			(lhs->value == rhs->value && lhs < rhs);
	}
};

template <class T>
std::ostream& operator<<(std::ostream& os, const node<T>& obj) {
	os << "{" << tr(&obj) << ": " << obj.value << "}";
	return os;
}