#pragma once
#include <set>
#include "graph.hpp"
#include "graph_match.hpp"

template <class T>
std::set<graph_match> search(const graph<T>& source, const graph<T>& target);

template <class T>
bool operator==(const graph<T>& lhs, const graph<T>& rhs);
template <class T>
bool operator!=(const graph<T>& lhs, const graph<T>& rhs);

#include "inline/graph_search.inl"