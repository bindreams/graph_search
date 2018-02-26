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
void short_test(const graph<T>& graph1, const graph<T>& graph2) {
	std::cout << "= Graph info ===================================================================" << std::endl;
	std::cout << "graph 1: " << graph1 << std::endl;
	std::cout << "graph 2: " << graph2 << std::endl;

	std::cout << std::endl << "= Results ======================================================================" << std::endl;
	auto mapping = graph1.contains(graph2);
	std::cout << "Answer: graph 1 " << (!mapping.empty() ? "contains" : "does not contain") << " graph 2" << std::endl;
	if (!mapping.empty()) std::cout << "Results:" << std::endl;
	for (auto&& i : mapping) {
		std::cout << "map " << i << std::endl;
	}
	std::cin.get();
	std::cout << std::endl << "= Benchmarks ===================================================================" << std::endl;
	std::ofstream ofs("last_test.json");
	json j;
	j["graph 1"] = graph1;
	j["graph 2"] = graph2;
	ofs << std::setw(4) << j;
	ofs.close();

	TIME(100, graph1.contains(graph2));
}

template<class T>
void full_test(const graph<T>& graph1, const graph<T>& graph2) {
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

	std::cout << std::endl << "= Results ======================================================================" << std::endl;
	auto mapping = graph1.contains(graph2);
	std::cout << "Answer: graph 1 " << (!mapping.empty() ? "contains" : "does not contain") << " graph 2" << std::endl;
	if (!mapping.empty()) std::cout << "Results:" << std::endl;
	for (auto&& i : mapping) {
		std::cout << "map " << i << std::endl;
	}
	
	std::cout << std::endl << "= Benchmarks ===================================================================" << std::endl;
	std::cout << "Size of puff 1: " << puff1.size() << " sectors (" << puff1.size_in_bytes() << " bytes)" << std::endl;
	std::cout << "Size of puff 2: " << puff2.size() << " sectors (" << puff2.size_in_bytes() << " bytes)" << std::endl;

	std::ofstream ofs("last_test.json");
	json j;
	j["graph 1"] = graph1;
	j["graph 2"] = graph2;
	ofs << std::setw(4) << j;
	ofs.close();

	TIME(100, puff<int> pf(graph1));
	TIME(100, puff<int> pf(graph2));
	TIME(100, puff1.contains(puff2));
	TIME(100, graph1.contains(graph2));
}

void full_test(const std::string& file) {
	std::ifstream ifs(file);
	json j;
	ifs >> j;

	graph<int> graph1 = j["graph 1"];
	graph<int> graph2 = j["graph 2"];

	full_test(graph1, graph2);
}

void short_test(const std::string& file) {
	std::ifstream ifs(file);
	json j;
	ifs >> j;

	graph<int> graph1 = j["graph 1"];
	graph<int> graph2 = j["graph 2"];

	short_test(graph1, graph2);
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

	full_test(graph1, graph2);
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

	full_test(graph1, graph2);
}

/*template <class T>
void add_to_report(json& j, const std::vector<T>& vec) {
	j.push_back(std::accumulate(vec.begin(), vec.end(), 0) / vec.size());
}

void incremental_report() {
	json j;

	auto key = random_graph(3, 0.2, 5);
	puff<int> key_p(key);

	for (int i = 1; i <= 20; i++) {
		std::cout << "i = " << i << std::endl;
		
		//containers for 100 tries with random graphs
		std::vector<long long> times;
		std::vector<size_t> sizes;
		std::vector<size_t> sinbs; //size in bytes
		std::vector<size_t> async_ctors;
		std::vector<size_t> async_contains;

		for (int j = 0; j < 100; j++) {
			auto graph = random_graph(i, 0.1, 5);
			long long rslt = 0;
			GET_TIME(100, graph.contains(key));

			puff<int> pf(graph);
			pf.contains(key);

			times.push_back(rslt);
			sizes.push_back(pf.size());
			sinbs.push_back(pf.size_in_bytes());
			async_ctors.push_back(pf.info.async_calls_ctor());
			async_contains.push_back(pf.info.async_calls_contains());
		}

		//std::cout << "    time: " << rslt << "mcs" << std::endl;

		add_to_report(j["time"], times);
		add_to_report(j["size"], sizes);
		add_to_report(j["size_in_bytes"], sinbs);
		add_to_report(j["async_calls_ctor"], async_ctors);
		add_to_report(j["async_calls_contains"], async_contains);
	}

	std::ofstream ofs("report-O0.json");	
	ofs << std::setw(4) << j;
	ofs.close();
}*/