#include "tests.hpp"
#include <iomanip>
#include <iostream>

int main() {
	std::cout << std::setw(2) << std::setfill('0') << std::boolalpha;

	test1();
}