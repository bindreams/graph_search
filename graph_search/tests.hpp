#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include <iomanip>

#include "json.hpp"
using json = nlohmann::json;

#include "benchmark.hpp"
#include "graph.hpp"
#include "puff.hpp"

template<class T>
void generic_test(const graph<T>& graph1, const graph<T>& graph2) {
	puff<T> puff1(graph1);
	puff<T> puff2(graph2);

	std::cout << "= Graph info ===================================================================" << std::endl;
	std::cout << "graph 1: " << graph1 << std::endl;
	std::cout << "puff 1: " << puff1 << std::endl;
	std::cout << "Made " << puff1.info.async_calls_ctor() << " asynchronous calls while building" << std::endl;
	puff1.contains(puff2);
	std::cout << "Made " << puff1.info.async_calls_contains() << " asynchronous calls while checking puff1.contains(puff2)" << std::endl;

	std::cout << "graph 2: " << graph2 << std::endl;
	std::cout << "puff 2: " << puff2 << std::endl;
	std::cout << "Made " << puff2.info.async_calls_ctor() << " asynchronous calls while building" << std::endl;
	puff2.contains(puff1);
	std::cout << "Made " << puff2.info.async_calls_contains() << " asynchronous calls while checking puff2.contains(puff1)" << std::endl;

	std::cout << "= Results and benchmarks =======================================================" << std::endl;
	auto mapping = graph1.contains(graph2);
	std::cout << "Answer: graph 1 " << (!mapping.empty() ? "contains" : "does not contain") << " graph 2" << std::endl;
	if (!mapping.empty()) std::cout << "Results:" << std::endl;
	for (auto&& i : mapping) {
		std::cout << "map " << i << std::endl;
	}
	
	std::cout << "Benchmarks:" << std::endl;
	std::cout << "Size of puff 1: " << puff1.size() << " sectors (" << puff1.size_in_bytes() << " bytes)" << std::endl;
	std::cout << "Size of puff 2: " << puff2.size() << " sectors (" << puff2.size_in_bytes() << " bytes)" << std::endl;

	std::ofstream ofs("last_test.json");
	json j;
	j["graph 1"] = graph1;
	j["graph 2"] = graph2;
	ofs << std::setw(4) << j;
	ofs.close();

	TIME(10000, puff<int> pf(graph1));
	TIME(10000, puff<int> pf(graph2));
	TIME(10000, puff1.contains(puff2));
	TIME(10000, graph1.contains(graph2));
}

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

	generic_test(graph1, graph2);
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

	generic_test(graph1, graph2);
}

void test3() {
	graph<int> graph1;
	auto n11 = graph1.attach(2);
	auto n12 = graph1.attach(1, {n11});
	auto n13 = graph1.attach(1, {n11, n12});
	auto n14 = graph1.attach(2, {n12});
	auto n15 = graph1.attach(2, {n13});

	graph<int> graph2;

	generic_test(graph1, graph2);
}

void test_from_file(const std::string& file) {
	std::ifstream ifs(file);
	json j;
	ifs >> j;

	graph<int> graph1 = j["graph 1"];
	graph<int> graph2 = j["graph 2"];

	generic_test(graph1, graph2);
}