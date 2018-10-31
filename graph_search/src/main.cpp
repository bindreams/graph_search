#include <iostream>
#include <limits>
#include "benchmark.hpp"
#include "tests.hpp"

int main(int argc, char** argv) {
	run_benchmark(argc, argv);

	std::cout << "Done." << std::endl;
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}