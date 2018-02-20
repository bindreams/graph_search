#pragma once
#include <algorithm>
#include <unordered_map>
#include <memory>

#include "graph_match.hpp"
#include "id_manager.hpp"
#include "node.hpp"

template <class T>
class graph_impl {
private:
	std::unordered_map<size_t, node<T>> nodes;
	id_manager ids;

public:
	size_t attach(const T& val, std::vector<size_t> connect_to = {});
	void detach(size_t id);

	template <class T>
	friend class puff;

	template <class T_>
	friend std::ostream& operator<<(std::ostream& os, const graph_impl<T_>& obj);
};

template<class T>
inline size_t graph_impl<T>::attach(const T& val, std::vector<size_t> connect_to) {
	size_t id = ids.get();

	nodes.emplace(id, node<T>(val, id));

	//Establish connections (edges)
	for (auto i : connect_to) {
		nodes.at(i).edges.push_back(&nodes.at(id));
		nodes.at(id).edges.push_back(&nodes.at(i));
	}

	return id;
}

template<class T>
inline void graph_impl<T>::detach(size_t id) {
	nodes.erase(id);
}

template <class T>
std::ostream& operator<<(std::ostream& os, const graph_impl<T>& obj) {
	os << "{" << std::endl;

	for (auto&& i : obj.nodes) {
		os << "    " << i.second << " -> ";

		for (auto&& connection : i.second.edges) {
			os << *connection << " ";
		}

		os << std::endl;
	}

	os << "}" << std::endl;

	return os;
}