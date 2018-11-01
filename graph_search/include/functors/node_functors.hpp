#pragma once

template <class T>
class node;

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