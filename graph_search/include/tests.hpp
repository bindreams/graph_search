#pragma once
#include <iostream>
#include <iomanip>
#include <fstream>
#include <iomanip>
#include "deps/json.hpp"
#include "graph_manip.hpp"
using json = nlohmann::json;

template<class T>
inline void test_search(const graph<T>& graph1, const graph<T>& graph2) {
	auto mapping = contains(graph1, graph2);

	std::cout
		<< "= Graph info ===================================================================" << std::endl
		<< "graph 1: " << graph1 << std::endl
		<< "graph 2: " << graph2 << std::endl
		<< std::endl
		<< "= Results ======================================================================" << std::endl
		<< "Answer: graph 1 " << (!mapping.empty() ? "contains" : "does not contain") << " graph 2" << std::endl;

	if (!mapping.empty()) {
		std::cout << "Results:" << std::endl;
		for (auto&& i : mapping) {
			std::cout << "map " << i << std::endl;
		}
	}

	std::ofstream ofs("last_test.json");
	json j;
	j["graph 1"] = graph1;
	j["graph 2"] = graph2;
	ofs << std::setw(4) << j;
	ofs.close();
}

inline void test_search(const std::string& file) {
	std::ifstream ifs(file);
	json j;
	ifs >> j;

	graph<int> graph1 = j["graph 1"];
	graph<int> graph2 = j["graph 2"];

	test_search(graph1, graph2);
}