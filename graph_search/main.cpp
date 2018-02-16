#include "tests.hpp"
#include <iomanip>
#include <iostream>
#include <thread>

int main() {
	std::cout << std::setw(2) << std::setfill('0') << std::boolalpha;
	std::cout << "HC: " << std::thread::hardware_concurrency();
	test1();
}