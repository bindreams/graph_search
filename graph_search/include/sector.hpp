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

	graph_match contains(const sector& other) const;
	sector& join_children(const sector& other);
	size_t size_in_bytes() const;

	constexpr sector() = default;
	sector(const sector& other) = default;
	sector(const node<T>* nd);
	sector(const sector* sec, const node<T>* nd);

	// Searches for all possible expansions by 1 node,
	// and returns a set of them.
	std::set<sector> expand() const;
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