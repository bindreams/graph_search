#pragma once
#include "random.hpp"

#include <stdexcept>
#include <string>
#include <iterator>
#include "deps/zh/random.hpp"
#include "deps/zh/iterator.hpp"

template <class T>
void push_random_edge(graph<T>& gr) {
	std::size_t max_edges = gr.size() * (gr.size() - 1) / 2;
	if (gr.count_edges() == max_edges) throw std::invalid_argument("push_random_edge: graph is complete");

	// Select it1 that is not yet connected to all nodes
	auto it1 = zh::select_from(gr.nodes());

	while (it1->edges().size() == gr.size() - 1) {
		it1++;
		if (it1 == gr.nodes().end()) it1 = gr.nodes().begin();
	}

	// Select it2 that is not connected to it1
	auto it2 = zh::select_from(gr.nodes());

	while (it2 == it1 || it2->edges().find(&*it1) != it2->edges().end()) {
		it2++;
		if (it2 == gr.nodes().end()) it2 = gr.nodes().begin();
	}

	gr.connect(it1, it2);
}

template <class T>
void pop_random_edge(graph<T>& gr) {
	if (gr.count_edges() == 0) throw std::invalid_argument("pop_random_edge: graph has no edges");

	auto it1 = zh::select_from(gr.nodes());

	// Select it1 that is connected to at least one node
	while (it1->edges().size() == 0) {
		it1++;
		if (it1 == gr.nodes().end()) it1 = gr.nodes().begin();
	}

	// Select a random connected node
	auto it2 = zh::select_from(it1->edges());

	gr.disconnect(*it1, **it2);
}

template <class T>
void mutate_edges(graph<T>& gr, double target_ratio) {
	if (target_ratio < 0 || target_ratio > 1)
		throw std::invalid_argument(std::string("mutate_edges: edges_ratio must be in range [0, 1] (got ") + std::to_string(target_ratio) + ")");

	std::size_t max_edges = gr.size() * (gr.size() - 1) / 2;
	std::size_t edges = gr.count_edges();
	std::size_t taredges = static_cast<std::size_t>(round(target_ratio * max_edges));

	if (edges < taredges) {
		while (edges < taredges) {
			push_random_edge(gr);
			edges++;
		}
	}
	else {
		while (edges > taredges) {
			pop_random_edge(gr);
			edges--;
		}
	}
}

template <class T, class Gen>
void push_random_node(graph<T>& gr, Gen&& value_gen) {
	gr.emplace(value_gen());
}

template <class T>
void push_random_node(graph<T>& gr) {
	push_random_node(gr, generator<T>());
}

template <class T>
void pop_random_node(graph<T>& gr) {
	if (gr.empty()) throw std::invalid_argument("pop_random_node: graph has no nodes");

	auto it = zh::select_from(gr.nodes());
	gr.erase(it);
}

template <class T, class Gen>
void mutate_nodes(graph<T>& gr, std::size_t target_size, Gen&& value_gen) {
	if (target_size > gr.size()) {
		for (std::size_t i = gr.size(); i < target_size; i++) {
			push_random_node<T>(gr, value_gen);
		}
	}
	else {
		for (std::size_t i = gr.size(); i > target_size; i--) {
			pop_random_node<T>(gr);
		}
	}
}

template <class T>
void mutate_nodes(graph<T>& gr, std::size_t target_size) {
	mutate_nodes(gr, target_size, generator<T>());
}

template <class T, class Gen>
void mutate(graph<T>& gr, std::size_t target_size, double target_ratio, Gen&& value_gen) {
	if (target_ratio < 0 || target_ratio > 1) 
		throw std::invalid_argument(std::string("mutate: edges_ratio must be in range [0, 1] (got ") + std::to_string(target_ratio) + ")");

	mutate_nodes(gr, target_size, value_gen);
	mutate_edges(gr, target_ratio);
}

template <class T>
void mutate(graph<T>& gr, std::size_t target_size, double target_ratio) {
	mutate(gr, target_size, target_ratio, generator<T>());
}