#pragma once
#include <algorithm>
#include <vector>
#include <vector>
#include <memory>

#include "graph_match.hpp"
#include "node.hpp"

template <class T>
class graph_impl {
private:
	std::vector<std::unique_ptr<node<T>>> nodes;

public:
	node<T>* attach(const T& val, std::vector<node<T>*> connect_to = {});
	void detach(const node<T>* nd);

	template <class T>
	friend class puff;

	template <class T_>
	friend std::ostream& operator<<(std::ostream& os, const graph_impl<T_>& obj);
};

template<class T>
inline node<T>* graph_impl<T>::attach(const T& val, std::vector<node<T>*> connect_to) {
	nodes.push_back(std::make_unique<node<T>>(val));

	//Establish connections (edges)
	for (auto i : connect_to) {
		i->edges.push_back(nodes.back().get());
		nodes.back()->edges.push_back(&(*i));
	}

	return nodes.back().get();
}

template<class T>
inline void graph_impl<T>::detach(const node<T>* nd) {
	auto pos = std::find_if(nodes.begin(), nodes.end(), [&](const std::unique_ptr<node<T>>& p) {return p.get() == nd; });
	nodes.erase(pos);
}

template <class T>
std::ostream& operator<<(std::ostream& os, const graph_impl<T>& obj) {
	os << "{" << std::endl;

	for (auto&& i : obj.nodes) {
		os << "    " << *i << " -> ";

		for (auto&& connection : i->edges) {
			os << *connection << " ";
		}

		os << std::endl;
	}

	os << "}" << std::endl;

	return os;
}