#pragma once
#include <cstdlib>

#include "graph_impl.hpp"

graph_impl<int> random_graph(size_t size, double avg_connections = 0.5, int max_value = RAND_MAX) {
	if (avg_connections < 0 || avg_connections > 1) throw std::invalid_argument("avg_connections must be a ratio in [0, 1]");

	graph_impl<int> rslt;

	//Building nodes
	for (size_t i = 0; i < size; i++) {
		rslt.attach(rand() % max_value);
	}

	//Attaching nodes
	for (auto i = rslt.nodes.begin(); i != std::prev(rslt.nodes.end()); i++) {
		for (auto j = std::next(i); j != rslt.nodes.end(); j++) {
			if (rand() % 10000 < avg_connections * 10000) {
				//i->second.
			}
		}
	}
}