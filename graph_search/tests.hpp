#pragma once
#include <iostream>
#include <iomanip>

#include "benchmark.hpp"
#include "graph.hpp"
#include "puff.hpp"

void test1() {
	graph<int> graph1;
	auto n11 = graph1.attach(1);
	auto n12 = graph1.attach(2, {n11});
	auto n13 = graph1.attach(3, {n11, n12});
	auto n14 = graph1.attach(4, {n12});
	auto n15 = graph1.attach(5, {n13});

	graph<int> graph2;
	auto n21 = graph2.attach(1);
	auto n22 = graph2.attach(2, {n21});
	auto n23 = graph2.attach(3, {n21, n22});

	puff<int> puff1(graph1);
	puff<int> puff2(graph2);

	std::cout << "graph_impl 1: " << graph1 << std::endl;
	std::cout << "puff 1: " << puff1 << std::endl;

	std::cout << "graph_impl 2: " << graph2 << std::endl;
	std::cout << "puff 2: " << puff2 << std::endl;

	std::cout << "================================================================================" << std::endl;
	std::cout << "Answer: graph 1 " << (graph1.contains(graph2).empty() ? "does not contain" : "contains") << " graph 2" << std::endl;
	std::cout << "Benchmarks:" << std::endl;
	std::cout << "Size of puff 1: " << puff1.size() << " sectors (" << puff1.size_in_bytes() << " bytes)" << std::endl;
	std::cout << "Size of puff 2: " << puff2.size() << " sectors (" << puff2.size_in_bytes() << " bytes)" << std::endl;

	TIME(10, puff<int> pf(graph1));
	TIME(10, puff<int> pf(graph2));
	TIME(10, puff1.contains(puff2));
	TIME(10, graph1.contains(graph2));
}

void test2() {
	graph<int> graph1;
	auto n11 = graph1.attach(2);
	auto n12 = graph1.attach(1, {n11});
	auto n13 = graph1.attach(1, {n11, n12});
	auto n14 = graph1.attach(2, {n12});
	auto n15 = graph1.attach(2, {n13});

	graph<int> graph2;
	auto n21 = graph2.attach(1);
	auto n22 = graph2.attach(2, {n21});

	puff<int> puff1(graph1);
	puff<int> puff2(graph2);

	std::cout << "graph_impl 1: " << graph1 << std::endl;
	std::cout << "puff 1: " << puff1 << std::endl;

	std::cout << "graph_impl 2: " << graph2 << std::endl;
	std::cout << "puff 2: " << puff2 << std::endl;

	std::cout << "================================================================================" << std::endl;
	auto matches = puff1.contains(puff2);
	std::cout << "Answer: graph 1 " << (graph1.contains(graph2).empty() ? "does not contain" : "contains") << " graph 2" << std::endl;
	if (!matches.empty()) {
		std::cout << "Can be mapped in " << matches.size() << " different ways:" << std::endl;
		for (auto&& i : matches) {
			std::cout << "map " << i << std::endl;
		}
	}

	std::cout << "Benchmarks:" << std::endl;
	std::cout << "Size of puff 1: " << puff1.size() << " sectors (" << puff1.size_in_bytes() << " bytes)" << std::endl;
	std::cout << "Size of puff 2: " << puff2.size() << " sectors (" << puff2.size_in_bytes() << " bytes)" << std::endl;

	TIME(10, puff<int> pf(graph1));
	TIME(10, puff<int> pf(graph2));
	TIME(10, puff1.contains(puff2));
	TIME(10, graph1.contains(graph2));
}