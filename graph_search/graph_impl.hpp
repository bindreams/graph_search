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
	void connect(size_t n1, size_t n2);
	void disconnect(size_t n1, size_t n2);

	size_t attach(const T& val, std::vector<size_t> connect_to = {});
	void detach(size_t id);

	const std::unordered_map<size_t, node<T>>& get_nodes() const;

	size_t count_edges() const;
	double ratio();
	size_t size();

	constexpr graph_impl() = default;

	template <class T_>
	friend void from_json(const json& j, graph_impl<T_>& obj);
};

template<class T>
inline void graph_impl<T>::connect(size_t n1, size_t n2) {
	if (n1 == n2) throw std::invalid_argument("cannot connect node to itself");

	nodes.at(n1).bi_connect(&nodes.at(n2));
}

template<class T>
inline void graph_impl<T>::disconnect(size_t n1, size_t n2) {
	nodes.at(n1).bi_disconnect(&nodes.at(n2));
}

template<class T>
inline size_t graph_impl<T>::attach(const T& val, std::vector<size_t> connect_to) {
	size_t id = ids.get();

	node<T>& element = nodes.emplace(id, node<T>(val, id)).first->second;

	//Establish connections (edges)
	for (auto i : connect_to) {
		element.bi_connect(&nodes.at(i));
	}

	return id;
}

template<class T>
inline void graph_impl<T>::detach(size_t id) {
	nodes.erase(id);
}

template<class T>
inline const std::unordered_map<size_t, node<T>>& graph_impl<T>::get_nodes() const {
	return nodes;
}

template<class T>
inline size_t graph_impl<T>::count_edges() const {
	size_t rslt = 0;

	for (auto&& i : nodes) {
		rslt += i.second.get_edges().size();
	}
	rslt /= 2;

	return rslt;
}

template<class T>
inline double graph_impl<T>::ratio() {
	size_t max_edges = size() * (size() - 1) / 2;
	size_t edges = g.count_edges();

	return static_cast<double>(edges) / max_edges;
}

template<class T>
inline size_t graph_impl<T>::size() {
	return nodes.size();
}

template <class T>
std::ostream& operator<<(std::ostream& os, const graph_impl<T>& obj) {
	os << "{" << std::endl;

	for (auto&& i : obj.get_nodes()) {
		os << "    " << i.second;
		if (!i.second.get_edges().empty()) os << " -> ";

		for (auto&& connection : i.second.get_edges()) {
			os << *connection << " ";
		}

		os << std::endl;
	}

	os << "}";

	return os;
}

template <class T>
void to_json(json& j, const graph_impl<T>& obj) {
	for (auto&& i : obj.get_nodes()) {
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
		node<T> n = i;
		
		size_t id = n.get_id();
		if (id > max_id) max_id = id;

		obj.nodes.emplace(id, std::move(n));
	}

	//Setting up id_manager
	obj.ids.set_next(max_id + 1);

	//Connecting
	for (auto&& i : j["nodes"]) {
		for (auto&& connect_id : i["edges"]) {
			obj.nodes[i["id"]].bi_connect(
				&obj.nodes[connect_id]);
		}
	}
}