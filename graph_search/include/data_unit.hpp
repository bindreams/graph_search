#pragma once
#include <cstdint>
#include "graph.hpp"
#include "puff.hpp"
#include "deps/json.hpp"
using json = nlohmann::json;

struct data_unit {
	// Keys
	std::size_t graph_size = 0;
	std::size_t max_puff_depth = 0;
	double target_graph_ratio = 1;

	// Data
	double graph_ratio = 1;
	double puff_depth = 0;
	double puff_edges = 0;
	double puff_sectors = 0;
	std::size_t averaged_over = 0;

	constexpr data_unit() = default;

	template <class T>
	data_unit(const graph<T>& gr, std::size_t max_depth, double target_ratio = -1);

	void merge(const data_unit& du);
};

void from_json(const json& j, data_unit& du);
void to_json(json& j, const data_unit& du);

// Definitions
template<class T>
inline data_unit::data_unit(const graph<T>& gr, std::size_t max_depth, double target_ratio) :
	graph_size(gr.size()),
	max_puff_depth(max_depth),
	graph_ratio(gr.ratio()),
	target_graph_ratio(target_ratio == -1 ? graph_ratio : target_ratio),
	averaged_over(1) {
	puff pf(gr, max_depth);

	puff_depth = pf.depth();
	puff_edges = pf.count_edges();
	puff_sectors = pf.count_sectors();
}
