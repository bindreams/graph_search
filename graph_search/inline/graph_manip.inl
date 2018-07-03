#pragma once
#include "graph_manip.hpp"
#include "generator.hpp"
#include "puff.hpp"

template <class T>
void push_random_edge(graph<T>& g) {
	std::size_t max_edges = g.size() * (g.size() - 1) / 2;
	if (g.count_edges() == max_edges) throw std::invalid_argument("push_random_edge: graph is complete");

	generator<std::size_t> gen;

	std::size_t id1 = gen(0, g.size());
	std::size_t id2 = gen(0, g.size());
	//cout << "Start with " << id1 << ", " << id2 << endl;

	//Select id1 that is not yet connected to all nodes
	while (g[id1].get_edges().size() == g.size() - 1) {
		id1++;
		if (id1 >= g.size()) id1 = 0;
	}
	//cout << " id1 corrected to " << id1 << endl;

	//Select id2 that is not connected to id1
	while (true) {
		//Ensure non-collision
		if (id2 == id1) id2++;
		if (id2 >= g.size()) id2 = 0;
		if (id2 == id1) id2++;
		//cout << " id2 corrected to " << id2 << endl;

		bool flag = false;

		for (auto&& i : g[id1].get_edges()) {
			if (i->get_id() == id2) {
				flag = true;
				break;
			}
		}

		if (flag == false) break;
		id2++;
	}
	//cout << "final: " << id1 << ", " << id2 << endl;

	g.connect(id1, id2);
}

template <class T>
void pop_random_edge(graph<T>& g) {
	if (g.count_edges() == 0) throw std::invalid_argument("pop_random_edge: graph is empty (no edges)");

	generator<std::size_t> gen;

	std::size_t id1 = gen(0, g.size());
	//cout << "Start with " << id1 << ", " << id2 << endl;

	//Select id1 that is connected to at least on node
	while (g[id1].get_edges().size() == 0) {
		id1++;
		if (id1 >= g.size()) id1 = 0;
	}
	//cout << " id1 corrected to " << id1 << endl;

	//Select a random connected node
	std::size_t t = gen(0, g[id1].get_edges().size());
	auto iter = g[id1].get_edges().begin();
	std::advance(iter, t);
	std::size_t id2 = (*iter)->get_id();

	g.disconnect(id1, id2);
}

template <class T>
void mutate_edges(graph<T>& g, double target_ratio) {
	if (target_ratio < 0 || target_ratio > 1) throw std::invalid_argument("mutate_edges: target_ratio must be a ratio in [0, 1]");

	std::size_t max_edges = g.size() * (g.size() - 1) / 2;
	std::size_t edges = g.count_edges();
	std::size_t target_edges = static_cast<std::size_t>(round(target_ratio * max_edges));

	if (edges < target_edges) {
		while (edges < target_edges) {
			push_random_edge(g);
			edges++;
		}
	}
	else {
		while (edges > target_edges) {
			pop_random_edge(g);
			edges--;
		}
	}
}

template <class T, class Gen>
void push_random_node(graph<T>& g, Gen value_gen) {
	g.push(value_gen());
}

template <class T>
void push_random_node(graph<T>& g) {
	push_random_node(g, generator<T>());
}

template <class T>
void pop_random_node(graph<T>& g) {
	if (g.empty()) throw std::invalid_argument("pop_random_node: graph has no nodes");

	g.pop(generator<std::size_t>()(0, g.size()));
}

template <class T, class Gen>
void mutate_nodes(graph<T>& g, std::size_t target_size, Gen value_gen) {
	if (target_size > g.size()) {
		for (std::size_t i = g.size(); i < target_size; i++) {
			push_random_node<T>(g, value_gen);
		}
	}
	else {
		for (std::size_t i = g.size(); i > target_size; i--) {
			pop_random_node<T>(g);
		}
	}
}

template <class T>
void mutate_nodes(graph<T>& g, std::size_t target_size) {
	mutate_nodes(g, target_size, generator<T>());
}

template <class T, class Gen>
void mutate(graph<T>& g, std::size_t target_size, double target_ratio, Gen value_gen) {
	if (target_ratio < 0 || target_ratio > 1) throw std::invalid_argument("mutate: edges_ratio must be a ratio in [0, 1]");

	mutate_nodes(g, target_size, value_gen);
	mutate_edges(g, target_ratio);
}

template <class T>
void mutate(graph<T>& g, std::size_t target_size, double target_ratio) {
	mutate(g, target_size, target_ratio, generator<T>());
}

template<class T>
inline std::set<graph_match> contains(const graph<T>& source, const graph<T>& target) {
	puff<T> target_puff(target);
	puff<T> source_puff(source, target_puff.depth());

	return source_puff.contains(target_puff);
}
