#include <benchmark/benchmark.h>
#include <fstream>
#include <filesystem>
#include <string>
#include "deps/json.hpp"
#pragma comment(lib, "shlwapi.lib") // MSVC++ does not work without this

#include "benchmark.hpp"
#include "deps/zh/extra_traits.hpp"
#include "util/generator.hpp"
#include "puff.hpp"
#include "graph_manip.hpp"

namespace fs = std::filesystem;
using json = nlohmann::json;

// Fix google macros to support namespaces, templates, and binds ==============
#define ZH_BENCHMARK_PRIVATE_NAME \
	BENCHMARK_PRIVATE_CONCAT(_benchmark_, BENCHMARK_PRIVATE_UNIQUE_ID, 0)

#define ZH_BENCHMARK_PRIVATE_DECLARE \
	static ::benchmark::internal::Benchmark* ZH_BENCHMARK_PRIVATE_NAME \
		BENCHMARK_UNUSED

#define ZH_BENCHMARK(...) \
	ZH_BENCHMARK_PRIVATE_DECLARE = \
		(::benchmark::internal::RegisterBenchmarkInternal( \
		new ::benchmark::internal::FunctionBenchmark(#__VA_ARGS__, __VA_ARGS__)))

#define ZH_BENCHMARK_BIND(...) \
	ZH_BENCHMARK_PRIVATE_DECLARE = \
		(::benchmark::internal::RegisterBenchmarkInternal( \
		new ::benchmark::internal::FunctionBenchmark(#__VA_ARGS__, \
		[](benchmark::State& state){__VA_ARGS__;})))

// ============================================================================
namespace { // internal linkage

// Generate a random graph
template <std::size_t graph_size, class GraphRatio>
void graph_creation(benchmark::State& state) {
	static_assert(zh::is_ratio_v<GraphRatio>, "benchmark: GraphRatio is not a valid ratio");
	double graph_ratio = static_cast<double>(GraphRatio::type::num) / GraphRatio::type::den;

	for (auto _ : state) {
		graph<int> g;
		mutate(g, graph_size, graph_ratio, test_gen());
	}
}

// Generate a random graph, then create a puff with specified max_depth
// Default depth is maximum possible depth
template <
	std::size_t graph_size, class GraphRatio,
	std::size_t max_depth = static_cast<std::size_t>(-1)>
void puff_creation(benchmark::State& state) {
	static_assert(zh::is_ratio_v<GraphRatio>, "benchmark: GraphRatio is not a valid ratio");
	double graph_ratio = static_cast<double>(GraphRatio::type::num) / GraphRatio::type::den;

	for (auto _ : state) {
		graph<int> g;
		mutate(g, graph_size, graph_ratio, test_gen());
		puff<int> pf(g, max_depth);
	}
}

// Load a graph from file, then create a puff with specified max_depth
// Default depth is maximum possible depth
template <std::size_t max_depth = static_cast<std::size_t>(-1)>
void puff_creation(benchmark::State& state, const fs::path& file) {
	std::ifstream ifs(file);
	json j;
	ifs >> j;
	graph<int> g = j;

	for (auto _ : state) {
		puff<int> pf(g, max_depth);
	}
}

// Search for a subgraph in a graph
template <
	std::size_t graph_size, class GraphRatio,
	std::size_t subgraph_size, class SubgraphRatio>
void graph_search(benchmark::State& state) {
	static_assert(zh::is_ratio_v<GraphRatio>, "benchmark: GraphRatio is not a valid ratio");
	static_assert(zh::is_ratio_v<SubgraphRatio>, "benchmark: SubgraphRatio is not a valid ratio");

	double graph_ratio = static_cast<double>(GraphRatio::type::num) / GraphRatio::type::den;
	double subgraph_ratio = static_cast<double>(SubgraphRatio::type::num) / SubgraphRatio::type::den;

	for (auto _ : state) {
		graph<int> x;
		mutate(x, graph_size, graph_ratio, test_gen());
		
		graph<int> y;
		mutate(y, subgraph_size, subgraph_ratio, test_gen());

		benchmark::DoNotOptimize(contains(x, y));
	}
}

} // namespace

// Active benchmarks ==========================================================
//ZH_BENCHMARK(graph_creation<10, std::ratio<2, 10>>);
//ZH_BENCHMARK(puff_creation<10, std::ratio<2, 10>, 3>);
ZH_BENCHMARK_BIND(puff_creation(state, "graph-15-0.2.json"))
->Unit(benchmark::kMillisecond)
->MinTime(60);
ZH_BENCHMARK_BIND(puff_creation(state, "graph-15-0.4.json"))
->Unit(benchmark::kMillisecond)
->MinTime(60);
ZH_BENCHMARK_BIND(puff_creation(state, "graph-15-0.6.json"))
->Unit(benchmark::kMillisecond)
->MinTime(60);
ZH_BENCHMARK_BIND(puff_creation(state, "graph-20-0.2.json"))
->Unit(benchmark::kMillisecond)
->MinTime(60);

// ============================================================================
int run_benchmark(int argc, char** argv) {
	benchmark::Initialize(&argc, argv);
	if (benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;
	benchmark::RunSpecifiedBenchmarks();
	return 0;
}