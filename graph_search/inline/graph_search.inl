#pragma once
#include "graph_search.hpp"
#include "puff.hpp"

template<class T>
inline std::set<graph_match> search(const graph<T>& source, const graph<T>& target) {
	puff<T> target_puff(target);
	puff<T> source_puff(source, target_puff.depth());

	return source_puff.search(target_puff);
}

template<class T>
inline bool operator==(const graph<T>& lhs, const graph<T>& rhs) {
	return puff(lhs) == puff(rhs);
}

template<class T>
inline bool operator!=(const graph<T>& lhs, const graph<T>& rhs) {
	return !(lhs == rhs);
}
