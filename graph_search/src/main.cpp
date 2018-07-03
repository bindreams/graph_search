#include <iostream>
#include <thread>

#include "report.hpp"
#include "tests.hpp"
#include "sorted_vector.hpp"
using namespace std;

int main() {
	std::cout << std::boolalpha;
	std::cout << "Threads available: " << std::thread::hardware_concurrency() << std::endl;
	
	std::string f = "D:/report9.json";

	graph<int> x;
	graph<int> y;

	mutate(x, 30, 0.2, test_gen());
	mutate(y, 3, 1, test_gen());

	short_test(x, y);

	cin.get();
}