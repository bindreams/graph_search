#include <benchmark/benchmark.h>
#pragma comment(lib, "shlwapi.lib")
#include "benchmark.hpp"
#include "deps/zh/extra_traits.hpp"
#include "util/generator.hpp"

#include "puff.hpp"
#include "graph_manip.hpp"

// Fix google macros to support namespaces and templates ======================
#define ZH_BENCHMARK_PRIVATE_NAME \
	BENCHMARK_PRIVATE_CONCAT(_benchmark_, BENCHMARK_PRIVATE_UNIQUE_ID, 0)

#define ZH_BENCHMARK_PRIVATE_DECLARE \
	static ::benchmark::internal::Benchmark* ZH_BENCHMARK_PRIVATE_NAME \
		BENCHMARK_UNUSED

#define ZH_BENCHMARK(...) \
	ZH_BENCHMARK_PRIVATE_DECLARE = \
		(::benchmark::internal::RegisterBenchmarkInternal( \
		new ::benchmark::internal::FunctionBenchmark(#__VA_ARGS__, __VA_ARGS__)))

// ============================================================================
namespace { // internal linkage

// namespace for benchmark functions
namespace bm {

// Generate a random graph
template <size_t graph_size, class GraphRatio>
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
	size_t graph_size, class GraphRatio,
	size_t max_depth = -1>
void puff_creation(benchmark::State& state) {
	static_assert(zh::is_ratio_v<GraphRatio>, "benchmark: GraphRatio is not a valid ratio");
	double graph_ratio = static_cast<double>(GraphRatio::type::num) / GraphRatio::type::den;

	for (auto _ : state) {
		graph<int> g;
		mutate(g, graph_size, graph_ratio, test_gen());
		puff<int> pf(g, max_depth);
	}
}

// Search for a subgraph in a graph
template <
	size_t graph_size, class GraphRatio,
	size_t subgraph_size, class SubgraphRatio>
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

} // namespace bm
} // namespace

// Active benchmarks ==========================================================
ZH_BENCHMARK(bm::graph_creation<10, std::ratio<2, 10>>);
ZH_BENCHMARK(bm::puff_creation<10, std::ratio<2, 10>, 3>);

// ============================================================================
int run_benchmark(int argc, char** argv) {
	benchmark::Initialize(&argc, argv);
	if (benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;
	benchmark::RunSpecifiedBenchmarks();
	return 0;
}