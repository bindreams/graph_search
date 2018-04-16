#include <iomanip>
#include <iostream>
#include <thread>

#include "random_graph.hpp"
#include "tests.hpp"

int main() {
	std::cout << std::boolalpha;
	std::cout << "Threads available: " << std::thread::hardware_concurrency() << std::endl;

	//auto x = random_graph(100, 0.2, 5);
	//auto y = random_graph(3, 1, 5);
	//short_test(x, y);

	short_test("test-20-3-true.json");

	//test1();
	//test2();
}