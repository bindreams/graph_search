#include <iostream>
#include <thread>

#include "graph_manip.hpp"
#include "tests.hpp"
using namespace std;

struct test_gen {
	generator<int> gen;

	int operator()() {
		return gen(0, 6);
	}
};

int main() {
	std::cout << std::boolalpha;
	std::cout << "Threads available: " << std::thread::hardware_concurrency() << std::endl;

	graph<int> x;
	graph<int> y;

	mutate(x, 30, 0.2, test_gen());
	mutate(y, 3, 1, test_gen());

	short_test("last_test.json");

	cin.get();
}