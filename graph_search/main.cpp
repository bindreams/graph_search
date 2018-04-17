#include <iomanip>
#include <iostream>
#include <thread>
#include <vector>

#include "random_graph.hpp"
#include "tests.hpp"
using namespace std;

template <size_t graph_size>
void report() {
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
	std::ifstream ifs("report.json");
	if (ifs.good()) {
		ifs >> base;
		ifs.close();
	}

	base[to_string(graph_size)] = rslt;

	std::ofstream ofs("report.json");
	ofs << std::setw(4) << base;
	ofs.close();
}

int main() {
	std::cout << std::boolalpha;
	std::cout << "Threads available: " << std::thread::hardware_concurrency() << std::endl;
	
	report<19>();

	//auto x = random_graph(10, 0.2, 5);
	//auto y = random_graph(3, 1, 5);
	//full_test(x, y);

	//short_test("test-20-3-true.json");

	//test1();
	//test2();
}