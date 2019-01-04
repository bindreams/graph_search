#pragma once
#include "report/data_unit.hpp"

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