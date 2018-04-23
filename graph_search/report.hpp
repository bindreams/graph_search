#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>

#include "json.hpp"
#include "graph.hpp"
#include "graph_manip.hpp"

using namespace std;

struct test_gen {
	generator<int> gen;

	int operator()() {
		return gen(0, 6);
	}
};

template <size_t graph_size>
void report(const std::string& filename) {
	const size_t sample_size = 100;
	const double start_ratio = 0.01;
	const double end_ratio = 0.2;
	const double step = 0.01;

	cout << "Building report for graph size " << graph_size << ":" << endl;

	json rslt;
	graph<int> graphs[sample_size] = {};

	//Fill vector
	for (size_t i = 0; i < sample_size; i++) {
		graphs[i] = random_graph(graph_size, start_ratio, 5);
	}

	for (double ratio = start_ratio; ratio <= end_ratio; ratio += step) {
		cout << "  edges ratio = " << ratio << endl;
		json temp;

		double avg_depth = 0;
		double avg_sectors = 0;
		double avg_puff_edges = 0;

		for (int i = 0; i < sample_size; i++) {
			grow_edges(graphs[i], step);
			puff<int> p(graphs[i]);

			avg_depth += p.depth();
			avg_sectors += p.count_sectors();
			avg_puff_edges += p.count_edges();
		}
		avg_depth /= sample_size;
		avg_sectors /= sample_size;
		avg_puff_edges /= sample_size;

		temp["graph_size"] = graph_size;
		temp["graph_edges"] = ratio;
		temp["puff_depth"] = avg_depth;
		temp["puff_sectors"] = avg_sectors;
		temp["puff_edges"] = avg_puff_edges;

		rslt.push_back(temp);
	}

	json base;
	std::ifstream ifs(filename);
	if (ifs.good()) {
		ifs >> base;
		ifs.close();
	}

	base[to_string(graph_size)] = rslt;

	std::ofstream ofs(filename);
	ofs << std::setw(4) << base;
	ofs.close();
}

void report2(double graph_ratio, const std::string& filename = "report.json") {
	const size_t sample_size = 100;
	const size_t start_size = 1;
	const size_t end_size = 19;
	const size_t step = 1;

	cout << "Building report for graph ratio " << graph_ratio << ":" << endl;

	json rslt;
	graph<int> graphs[sample_size] = {};

	//Fill vector
	for (size_t i = 0; i < sample_size; i++) {
		mutate<int, test_gen>(graphs[i], start_size, graph_ratio);
	}

	for (size_t size = start_size; size <= end_size; size += step) {
		cout << "  size = " << size << endl;
		json temp;

		double avg_depth = 0;
		double avg_sectors = 0;
		double avg_puff_edges = 0;

		for (int i = 0; i < sample_size; i++) {
			mutate(graphs[i], size, graph_ratio);
			puff<int> p(graphs[i], 11);

			avg_depth += p.depth();
			avg_sectors += p.count_sectors();
			avg_puff_edges += p.count_edges();
		}
		avg_depth /= sample_size;
		avg_sectors /= sample_size;
		avg_puff_edges /= sample_size;

		temp["graph_size"] = size;
		temp["graph_ratio"] = graph_ratio;
		temp["puff_depth"] = avg_depth;
		temp["puff_sectors"] = avg_sectors;
		temp["puff_edges"] = avg_puff_edges;

		rslt.push_back(temp);
	}

	json base;
	std::ifstream ifs(filename);
	if (ifs.good()) {
		ifs >> base;
		ifs.close();
	}

	for (auto i : rslt)
		base.push_back(i);

	std::ofstream ofs(filename);
	ofs << std::setw(4) << base;
	ofs.close();
}