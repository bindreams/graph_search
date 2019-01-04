#pragma once
// These are node functors, required for puff and sector
// Functors used in graph and node are declared in graph/node/node_functors.hpp

template <class T>
class node;

template<class T>
struct node_value_equal {
	inline bool operator() (const node<T>* const& lhs, const node<T>* const& rhs) const;
};

template<class T>
struct node_value_order {
	inline bool operator() (const node<T>* const& lhs, const node<T>* const& rhs) const;
};

#include "node_functors.inl"