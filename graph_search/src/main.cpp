#include <iostream>
#include <limits>
#include "benchmark.hpp"
#include "tests.hpp"

int main(int argc, char** argv) {
	//int status = run_benchmark(argc, argv);

	test_search("last_test.json");

	std::cout << "Done." << std::endl;
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	return 0;
}