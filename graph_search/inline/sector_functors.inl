#pragma once
#include "functors/sector_functors.hpp"
#include "node_group.hpp"

template<class T>
inline bool sector_lexicographical_order<T>::operator()(sector<T> const & lhs, sector<T> const & rhs) const {
	return std::lexicographical_compare(
		lhs.nodes.begin(), lhs.nodes.end(),
		rhs.nodes.begin(), rhs.nodes.end(),
		node_value_order<T>());
}

template<class T>
inline bool sector_lexicographical_order<T>::operator()(const sector<T>* const & lhs, const sector<T>* const & rhs) const {
	return operator()(*lhs, *rhs);
}