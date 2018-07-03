#pragma once
#include <iostream>
#include <fstream>
#include <iomanip>

#include "deps/json.hpp"
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

puff<int> mk(const graph<int>& gr, size_t max_depth) {
	return puff<int>(gr, max_depth);
}

void report2(double graph_ratio, const std::string& filename = "report.json") {
	const unsigned int max_depth = 9; // Max puff depth
	const unsigned int sample_size = 50; // Sample size for averaging
	const unsigned int batch_size = 5; // Size of parralelizable batches
	const unsigned int start_size = 1;
	const unsigned int end_size = 160;
	const unsigned int step = 2;

	static_assert(sample_size % batch_size == 0, "report2: sample_size must be divisible by batch_size");

	cout << "Building report for graph ratio " << graph_ratio << ":" << endl;

	graph<int> graphs[sample_size] = {};

	for (size_t size = start_size; size <= end_size; size += step) {
		cout << "  size = " << size << endl;
		json temp;

		double avg_depth = 0;
		double avg_sectors = 0;
		double avg_puff_edges = 0;

		for (int i = 0; i < sample_size; i++) {
			mutate(graphs[i], size, graph_ratio);
		}

		for (int i = 0; i < sample_size / batch_size; i++) {
			std::array<std::future<puff<int>>, batch_size> puffs = {};

			for (int j = 0; j < batch_size; j++) {
				puffs[j] = std::async(
					std::launch::async, &mk, std::cref(graphs[batch_size*i + j]), max_depth);
			}

			for (int j = 0; j < batch_size; j++) {
				puff<int> p(puffs[j].get());

				avg_depth += p.depth();
				avg_sectors += p.count_sectors();
				avg_puff_edges += p.count_edges();
			}
		}

		avg_depth /= sample_size;
		avg_sectors /= sample_size;
		avg_puff_edges /= sample_size;

		temp["graph_size"] = size;
		temp["graph_ratio"] = graph_ratio;
		temp["puff_depth"] = avg_depth;
		temp["puff_sectors"] = avg_sectors;
		temp["puff_edges"] = avg_puff_edges;

		json base;
		std::ifstream ifs(filename);
		if (ifs.good()) {
			ifs >> base;
			ifs.close();
		}

		base.push_back(temp);

		std::ofstream ofs(filename);
		ofs << std::setw(4) << base;
		ofs.close();
	}
}