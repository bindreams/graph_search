#pragma once
#include <benchmark/benchmark.h>
#include "deps/zh/extra_traits.hpp"

// Run Google Benchmark
// All benchmarks are defined in benchmark.cpp
// Return 0 on success
int run_benchmark(int argc, char** argv);