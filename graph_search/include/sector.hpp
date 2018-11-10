#pragma once
#include <algorithm>
#include <set>
#include <boost/container/flat_set.hpp>

#include "functors/sector_functors.hpp"
#include "node_group.hpp"
#include "graph_match.hpp"
using boost::container::flat_set;

template <class T>
class sector {
public:
	node_group<T> nodes;
	flat_set<const sector*, sector_lexicographical_order<T>> children;

	graph_match search(const sector& other) const;
	sector& join_children(const sector& other);
	std::size_t size_in_bytes() const;

	constexpr sector() = default;
	sector(const sector& other) = default;
	// Create a sector from one node
	explicit sector(const node<T>& nd);
	// Construct a sector by expanding a sector by one node
	sector(const sector<T>& sec, const node<T>& nd);
	// Construct a sector by merging two sectors and setting them as children.
	// This assumes that children have identical node_groups except for one node.
	sector(const sector& child1, const sector& child2);
};

template<class T> inline bool operator==(const sector<T>& lhs, const sector<T>& rhs);
template<class T> inline bool operator!=(const sector<T>& lhs, const sector<T>& rhs);
template<class T> inline bool operator< (const sector<T>& lhs, const sector<T>& rhs);
template<class T> inline bool operator> (const sector<T>& lhs, const sector<T>& rhs);
template<class T> inline bool operator<=(const sector<T>& lhs, const sector<T>& rhs);
template<class T> inline bool operator>=(const sector<T>& lhs, const sector<T>& rhs);

template <class T>
std::ostream& operator<<(std::ostream& os, const sector<T>& obj);

#include "inline/sector.inl"