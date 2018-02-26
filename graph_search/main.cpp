#include <iomanip>
#include <iostream>
#include <thread>

#include "random_graph.hpp"
#include "tests.hpp"

int main() {
	std::cout << std::setw(2) << std::setfill('0') << std::boolalpha;
	std::cout << "Threads available: " << std::thread::hardware_concurrency() << std::endl;

	//auto x = random_graph(500, 0.2, 5);
	//auto y = random_graph(3, 1, 5);
	//short_test(x, y);

	full_test("test-branch-3.json");

	//test1();
	//test2();

	//test_from_file("save.json");

	//std::cout << random_graph(1000, 0.2, 5);
	//std::cin.get();
}