#pragma once
#include <unordered_map>

#include "json.hpp"
using json = nlohmann::json;

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

	template <class T_>
	friend void to_json(json& j, const graph_impl<T_>& obj);

	template <class T_>
	friend void from_json(const json& j, graph_impl<T_>& obj);

	friend graph_impl<int> random_graph(size_t size, double avg_connections, int max_value);
};

template<class T>
inline size_t graph_impl<T>::attach(const T& val, std::vector<size_t> connect_to) {
	size_t id = ids.get();

	nodes.emplace(id, node<T>(val, id));

	//Establish connections (edges)
	for (auto i : connect_to) {
		nodes.at(i).edges.emplace(&nodes.at(id));
		nodes.at(id).edges.emplace(&nodes.at(i));
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

template <class T>
void to_json(json& j, const graph_impl<T>& obj) {
	for (auto&& i : obj.nodes) {
		j["nodes"].push_back(i.second);
	}
}

template <class T>
void from_json(const json& j, graph_impl<T>& obj) {
	//Clearing
	obj.nodes.clear();

	//Adding all nodes
	size_t max_id = 0;
	for (auto&& i : j["nodes"]) {
		node<T> n = i.get<node<T>>();
		
		size_t id = n.get_id();
		if (id > max_id) max_id = id;

		obj.nodes.emplace(id, std::move(n));
	}

	//Setting up id_manager
	obj.ids.set_next(max_id + 1);

	//Connecting
	for (auto&& i : j["nodes"]) {
		for (auto&& connect_id : i["edges"]) {
			obj.nodes[i["id"]]
				.edges.emplace(&obj.nodes[connect_id.get<size_t>()]);
			obj.nodes[connect_id.get<size_t>()]
				.edges.emplace(&obj.nodes[i["id"]]);
		}
	}
}