#pragma once
#include <algorithm>
#include "node_functors.hpp"

template <class T>
class sector;

template<class T>
struct sector_lexicographical_order {
	bool operator() (sector<T> const& lhs, sector<T> const& rhs) const;
	bool operator() (const sector<T>* const& lhs, const sector<T>* const& rhs) const;
};

#include "inline/sector_functors.inl"