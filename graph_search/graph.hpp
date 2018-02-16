#pragma once
#include <set>
#include "graph_impl.hpp"
#include "graph_match.hpp"
#include "puff.hpp"

template <typename T>
class graph : public graph_impl<T> {
public:
	std::set<graph_match> contains(const graph& other) const;
};

template<typename T>
inline std::set<graph_match> graph<T>::contains(const graph& other) const {
	puff<T> other_puff(other);
	puff<T> my_puff(*this, other_puff.depth());

	//std::cout << other_puff.depth();

	return my_puff.contains(other_puff);
}
