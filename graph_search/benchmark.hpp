#pragma once
#include "puff.hpp"

#include <chrono>
#include <iostream>
#include <string>
#include <map>
#include <thread>

namespace ch = std::chrono;

void time_print(const std::string& msg, const long long& time) {
	std::string str_time = std::to_string(time);
	str_time += "mcs";

	std::cout << msg;
	if (msg.length() + str_time.length() > 80 - 1 - 1) std::cout << std::endl << std::string(80 - 1 - str_time.length(), ' ');
	else std::cout << std::string(80 - 1 - msg.length() - str_time.length(), ' ');
	std::cout << str_time << std::endl;
}

#define TIME(times, ...) { \
	std::size_t TIME_N = times; \
	auto TIME_t1 = ch::steady_clock::now(); \
	for (std::size_t i = 0; i < TIME_N; i++) { \
		__VA_ARGS__; \
	} \
	\
	auto TIME_t2 = ch::steady_clock::now(); \
	auto TIME_time = ch::duration_cast<ch::microseconds>(TIME_t2 - TIME_t1).count() / TIME_N; \
	\
	std::string TIME_msg = "Timed "; \
	TIME_msg += #__VA_ARGS__; \
	time_print(TIME_msg, TIME_time); \
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