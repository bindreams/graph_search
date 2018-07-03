#include <iostream>
#include <thread>

#include "report.hpp"
#include "tests.hpp"
using namespace std;

int main() {
	std::cout << std::boolalpha;
	std::cout << "Threads available: " << std::thread::hardware_concurrency() << std::endl;
	
	std::string f = "D:/report9.json";

	graph<int> x;
	graph<int> y;

	mutate<int, test_gen>(x, 30, 0.2);
	mutate<int, test_gen>(y, 3, 1);

	short_test(x, y);

	cin.get();
}