#pragma once
#include <cstdlib>
#include "graph.hpp"

template <class T>
void push_random_edge(graph<T>& g);

template <class T>
void pop_random_edge(graph<T>& g);

template <class T>
void mutate_edges(graph<T>& g, double target_ratio);

template <class T, class Gen>
void push_random_node(graph<T>& g, Gen&& value_gen);

template <class T>
void push_random_node(graph<T>& g);

template <class T>
void pop_random_node(graph<T>& g);

template <class T, class Gen>
void mutate_nodes(graph<T>& g, std::size_t target_size, Gen&& value_gen);

template <class T>
void mutate_nodes(graph<T>& g, std::size_t target_size);

template <class T, class Gen>
void mutate(
	graph<T>& g, std::size_t target_size, double target_ratio, Gen&& value_gen);

template <class T>
void mutate(graph<T>& g, std::size_t target_size, double target_ratio);

#include "inline/graph_random.inl"