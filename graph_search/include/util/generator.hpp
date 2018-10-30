#pragma once
#include <random>
#include <limits>
#include <ctime>

template <class T>
struct generator {
	static std::mt19937 device;

	T operator()(const T& min = std::numeric_limits<T>::lowest(), const T& max = std::numeric_limits<T>::max());
};

template <class T>
std::mt19937 generator<T>::device(static_cast<unsigned int>(std::time(nullptr)));

template <class T>
inline T generator<T>::operator()(const T& min, const T& max) {
	if constexpr(std::is_integral_v<T>)
		return std::uniform_int_distribution<T>(min, max-1)(device);

	if constexpr(std::is_floating_point_v<T>)
		return std::uniform_real_distribution<T>(min, max)(device);
}

struct test_gen {
	generator<int> gen;

	int operator()() {
		return gen(0, 6);
	}
};