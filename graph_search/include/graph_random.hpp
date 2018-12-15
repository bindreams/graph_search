#pragma once
#include <cstdlib>
#include "graph.hpp"

template <class T>
void push_random_edge(graph<T>& gr);

template <class T>
void pop_random_edge(graph<T>& gr);

template <class T>
void mutate_edges(graph<T>& gr, double target_ratio);

template <class T, class Gen>
void push_random_node(graph<T>& gr, Gen&& value_gen);

template <class T>
void push_random_node(graph<T>& gr);

template <class T>
void pop_random_node(graph<T>& gr);

template <class T, class Gen>
void mutate_nodes(graph<T>& gr, std::size_t target_size, Gen&& value_gen);

template <class T>
void mutate_nodes(graph<T>& gr, std::size_t target_size);

template <class T, class Gen>
void mutate(
	graph<T>& gr, std::size_t target_size, double target_ratio, Gen&& value_gen);

template <class T>
void mutate(graph<T>& gr, std::size_t target_size, double target_ratio);

#include "inline/graph_random.inl"