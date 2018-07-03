#pragma once
#include "puff.hpp"

#include <chrono>
#include <iostream>
#include <string>
#include <map>
#include <thread>

namespace ch = std::chrono;

void print_benchmark(const std::string& name, const ch::microseconds& time) {
	std::string message = "Timed ";
	message += name;

	std::string time_str = std::to_string(time.count());
	time_str += " mcs";

	std::cout << message;
	if (message.length() + time_str.length() > 80 - 1 - 1) std::cout << std::endl << std::string(80 - 1 - time_str.length(), ' ');
	else std::cout << std::string(80 - 1 - message.length() - time_str.length(), ' ');
	std::cout << time_str << std::endl;
}

#define TIME(times, ...) { \
	std::size_t TIME_N = times; \
	auto TIME_t1 = ch::steady_clock::now(); \
	for (std::size_t i = 0; i < TIME_N; i++) { \
		__VA_ARGS__; \
	} \
	\
	auto TIME_t2 = ch::steady_clock::now(); \
	ch::microseconds TIME_time = ch::duration_cast<ch::microseconds>(TIME_t2 - TIME_t1) / TIME_N; \
	print_benchmark(#__VA_ARGS__, TIME_time); \
}

/*#define GET_TIME(times, ...) { \
std::size_t TIME_N = times; \
auto TIME_t1 = ch::steady_clock::now(); \
for (std::size_t i = 0; i < TIME_N; i++) { \
__VA_ARGS__; \
} \
\
auto TIME_t2 = ch::steady_clock::now(); \
rslt = ch::duration_cast<ch::microseconds>(TIME_t2 - TIME_t1).count() / TIME_N; \
}*/