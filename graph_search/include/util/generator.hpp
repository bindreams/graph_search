#pragma once
#include "deps/zh/random.hpp"

struct test_gen {
	generator<int> gen;

	int operator()() {
		return gen(0, 5);
	}
};