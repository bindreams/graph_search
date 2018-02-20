#include <iomanip>
#include <iostream>
#include <thread>

#include "random_graph.hpp"
#include "tests.hpp"

int main() {
	std::cout << std::setw(2) << std::setfill('0') << std::boolalpha;
	std::cout << "Threads available: " << std::thread::hardware_concurrency() << std::endl;
	test4();
}