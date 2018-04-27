#pragma once
#include <cstdlib>

#include "graph.hpp"
#include "generator.hpp"

template <class T>
void push_edge(graph<T>& g) {
	size_t max_edges = g.size() * (g.size() - 1) / 2;
	if (g.count_edges() == max_edges) throw std::invalid_argument("add_random_edge: graph is complete");

	generator<size_t> gen;

	size_t id1 = gen(0, g.size());
	size_t id2 = gen(0, g.size());
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
void pop_edge(graph<T>& g) {
	if (g.count_edges() == 0) throw std::invalid_argument("add_random_edge: graph is empty (no edges)");

	generator<size_t> gen;

	size_t id1 = gen(0, g.size());
	//cout << "Start with " << id1 << ", " << id2 << endl;

	//Select id1 that is connected to at least on node
	while (g[id1].get_edges().size() == 0) {
		id1++;
		if (id1 >= g.size()) id1 = 0;
	}
	//cout << " id1 corrected to " << id1 << endl;

	//Select a random connected node
	size_t t = gen(0, g[id1].get_edges().size());
	auto iter = g[id1].get_edges().begin();
	std::advance(iter, t);
	size_t id2 = (*iter)->get_id();

	g.disconnect(id1, id2);
}

template <class T, class Gen = generator<T>>
void push_nodes(graph<T>& g, size_t count = 1) {
	for (size_t i = 0; i < count; i++) {
		g.push(Gen()());
	}
}

template <class T>
void pop_nodes(graph<T>& g, size_t count = 1) {
	if (count > g.size()) throw std::invalid_argument("pop_nodes: graph does not have enough nodes");

	generator<size_t> gen;

	for (size_t i = 0; i < count; i++) {
		g.pop(gen(0, g.size()));
	}
}

template <class T>
void mutate_edges(graph<T>& g, double target_ratio) {
	if (target_ratio < 0 || target_ratio > 1) throw std::invalid_argument("mutate_edges: target_ratio must be a ratio in [0, 1]");

	size_t max_edges = g.size() * (g.size() - 1) / 2;
	size_t edges = g.count_edges();
	size_t target_edges = static_cast<size_t>(round(target_ratio * max_edges));
	
	if (edges < target_edges) {
		while (edges < target_edges) {
			push_edge(g);
			edges++;
		}
	}
	else {
		while (edges > target_edges) {
			pop_edge(g);
			edges--;
		}
	}
}

template <class T, class Gen = generator<T>>
void mutate(graph<T>& g, size_t target_size, double target_ratio) {
	if (target_ratio < 0 || target_ratio > 1) throw std::invalid_argument("random_graph: edges_ratio must be a ratio in [0, 1]");
	
	if (target_size > g.size()) {
		push_nodes<T, Gen>(g, target_size - g.size());
	}
	else {
		pop_nodes<T>(g, g.size() - target_size);
	}

	mutate_edges(g, target_ratio);
}