#pragma once
#include <iostream>
using std::cout;
using std::endl;
#include <iomanip>
#include <fstream>
#include <iomanip>

#include "deps/json.hpp"
using json = nlohmann::json;

#include "benchmark.hpp"
#include "graph.hpp"
#include "puff.hpp"

template<class T>
void short_test(const graph<T>& graph1, const graph<T>& graph2) {
	cout << "= Graph info ===================================================================" << endl;
	cout << "graph 1: " << graph1 << endl;
	cout << "graph 2: " << graph2 << endl;

	cout << endl << "= Results ======================================================================" << endl;
	auto mapping = contains(graph1, graph2);
	cout << "Answer: graph 1 " << (!mapping.empty() ? "contains" : "does not contain") << " graph 2" << endl;
	if (!mapping.empty()) cout << "Results:" << endl;
	for (auto&& i : mapping) {
		cout << "map " << i << endl;
	}

	cout << endl << "= Benchmarks ===================================================================" << endl;
	std::ofstream ofs("last_test.json");
	json j;
	j["graph 1"] = graph1;
	j["graph 2"] = graph2;
	ofs << std::setw(4) << j;
	ofs.close();

	TIME(100, contains(graph1, graph2));
}

template<class T>
void full_test(const graph<T>& graph1, const graph<T>& graph2) {
	puff<T> puff1(graph1);
	puff<T> puff2(graph2);

	cout << "= Graph info ===================================================================" << endl;
	cout << "graph 1: " << graph1 << endl;
	cout << "puff 1: " << puff1 << endl;
	cout << "Made " << puff1.info.async_calls_ctor() << " asynchronous calls while building" << endl;
	puff1.contains(puff2);
	cout << "Made " << puff1.info.async_calls_contains() << " asynchronous calls while checking puff1.contains(puff2)" << endl;

	cout << "graph 2: " << graph2 << endl;
	cout << "puff 2: " << puff2 << endl;
	cout << "Made " << puff2.info.async_calls_ctor() << " asynchronous calls while building" << endl;
	puff2.contains(puff1);
	cout << "Made " << puff2.info.async_calls_contains() << " asynchronous calls while checking puff2.contains(puff1)" << endl;

	cout << endl << "= Results ======================================================================" << endl;
	auto mapping = contains(graph1, graph2);
	cout << "Answer: graph 1 " << (!mapping.empty() ? "contains" : "does not contain") << " graph 2" << endl;
	if (!mapping.empty()) cout << "Results:" << endl;
	for (auto&& i : mapping) {
		cout << "map " << i << endl;
	}
	
	cout << endl << "= Benchmarks ===================================================================" << endl;
	cout << "Size of puff 1: " << puff1.count_sectors() << " sectors, " << puff1.count_edges() << " edges, " << puff1.size_in_bytes() << " bytes" << endl;
	cout << "Size of puff 2: " << puff2.count_sectors() << " sectors, " << puff2.count_edges() << " edges, " << puff2.size_in_bytes() << " bytes" << endl;

	std::ofstream ofs("last_test.json");
	json j;
	j["graph 1"] = graph1;
	j["graph 2"] = graph2;
	ofs << std::setw(4) << j;
	ofs.close();

	TIME(100, puff<int> pf(graph1));
	TIME(100, puff<int> pf(graph2));
	TIME(100, puff1.contains(puff2));
	TIME(100, contains(graph1, graph2));
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
	auto n11 = graph1.push(1);
	auto n12 = graph1.push(2, {n11});
	auto n13 = graph1.push(3, {n11, n12});
	auto n14 = graph1.push(4, {n12});
	auto n15 = graph1.push(5, {n13});

	graph<int> graph2;
	auto n21 = graph2.push(1);
	auto n22 = graph2.push(2, {n21});
	auto n23 = graph2.push(3, {n21, n22});

	full_test(graph1, graph2);
}

void test2() {
	graph<int> graph1;
	auto n11 = graph1.push(2);
	auto n12 = graph1.push(1, {n11});
	auto n13 = graph1.push(1, {n11, n12});
	auto n14 = graph1.push(2, {n12});
	auto n15 = graph1.push(2, {n13});

	graph<int> graph2;
	auto n21 = graph2.push(1);
	auto n22 = graph2.push(2, {n21});

	full_test(graph1, graph2);
}