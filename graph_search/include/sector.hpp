#pragma once
#include <algorithm>
#include <set>
#include <boost/container/flat_set.hpp>
#include "node.hpp"
#include "graph_match.hpp"
using boost::container::flat_set;

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

template <class T>
class sector {
public:
	flat_set<const node<T>*, node_value_order<T>> nodes;
	flat_set<const sector<T>*, sector_lexicographical_order<T>> children;

	graph_match contains(const sector& other) const;
	sector& join_children(const sector& other);
	size_t size_in_bytes() const;

	constexpr sector() = default;
	sector(const sector<T>& other) = default;
	sector(const node<T>* nd);
	sector(const sector<T>* sec, const node<T>* nd);

	// Searches for all possible expansions by 1 node,
	// and returns a set of them.
	std::set<sector<T>> expand() const;
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