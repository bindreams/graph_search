#pragma once
#include <algorithm>
#include "node_functors.hpp"

template <class T>
class sector;

template<class T>
struct sector_lexicographical_order {
	inline bool operator() (sector<T> const& lhs, sector<T> const& rhs) const {
		return std::lexicographical_compare(
			lhs.nodes.begin(), lhs.nodes.end(),
			rhs.nodes.begin(), rhs.nodes.end(),
			node_value_order<T>());
	}

	inline bool operator() (const sector<T>* const& lhs, const sector<T>* const& rhs) const {
		return operator()(*lhs, *rhs);
	}
};

template<class T>
struct sector_nodes_hash {
	std::size_t operator()(const sector<T>& obj) const {
		return std::hash<node_group<T>>()(obj.nodes);
	}
};

template<class T>
struct sector_nodes_equal {
	std::size_t operator()(const sector<T>& lhs, const sector<T>& rhs) const {
		return lhs.nodes == rhs.nodes;
	}
};
