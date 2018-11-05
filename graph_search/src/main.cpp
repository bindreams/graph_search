#include <iostream>
#include <limits>
#include "benchmark.hpp"
#include "tests.hpp"

int main(int argc, char** argv) {
	/*std::ifstream ifs("graph-20-0.2.json");
	json j;
	ifs >> j;
	graph<int> g = j;

	puff pf(g);

	for (std::size_t i = 0; i < pf.depth(); i++) {
		std::cout << pf[i].size() << std::endl;
	}*/

	run_benchmark(argc, argv);
	//test_search("last_test.json");

	/*graph<int> g;
	mutate(g, 15, 0.6);

	std::ofstream ofs("graph-15-0.6.json");
	json j = g;
	ofs << j;*/

	std::cout << "Done." << std::endl;
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}