#pragma once
#include <cstdlib>

#include "graph.hpp"

template <class T>
void grow_edges(graph<T>& g, double grow_by) {
	if (grow_by < 0 || grow_by > 1) throw std::invalid_argument("grow_by must be a ratio in [0, 1]");

	size_t max_edges = g.get_nodes().size() * (g.get_nodes().size() - 1) / 2;
	size_t edges = g.count_edges();
	size_t edges_to_grow = round(grow_by * max_edges);

	if (edges + edges_to_grow > max_edges) throw std::invalid_argument("can not grow by that many edges");

	for (size_t i = 0; i < edges_to_grow; i++) {
		size_t id1 = rand() % g.get_nodes().size();
		size_t id2 = rand() % g.get_nodes().size();
		//cout << "Start with " << id1 << ", " << id2 << endl;

		//Select id1 that is not yet connected to all nodes
		while (g.get_nodes().at(id1).get_edges().size() == g.get_nodes().size() - 1) {
			id1++;
			if (id1 >= g.get_nodes().size()) id1 = 0;
		}
		//cout << " id1 corrected to " << id1 << endl;

		//Select id2 that is not connected to id1
		while (true) {
			//Ensure non-collision
			if (id2 == id1) id2++;
			if (id2 >= g.get_nodes().size()) id2 = 0;
			if (id2 == id1) id2++;
			//cout << " id2 corrected to " << id2 << endl;

			bool flag = false;

			for (auto&& i : g.get_nodes().at(id1).get_edges()) {
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
}

graph<int> random_graph(size_t size, double edges_ratio = 0.5, int max_value = RAND_MAX) {
	if (edges_ratio < 0 || edges_ratio > 1) throw std::invalid_argument("edges_ratio must be a ratio in [0, 1]");

	graph<int> rslt;

	//Building nodes
	for (size_t i = 0; i < size; i++) {
		rslt.attach(rand() % max_value);
	}

	grow_edges(rslt, edges_ratio);

	return rslt;
}