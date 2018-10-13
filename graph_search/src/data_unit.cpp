#include "report/data_unit.hpp"

void data_unit::merge(const data_unit & du) {
	assert(graph_size == du.graph_size);
	assert(max_puff_depth == du.max_puff_depth);
	assert(target_graph_ratio == du.target_graph_ratio);

	int new_avg = averaged_over + du.averaged_over;

	graph_ratio =
		   graph_ratio * (   averaged_over / static_cast<double>(new_avg)) +
		du.graph_ratio * (du.averaged_over / static_cast<double>(new_avg));

	puff_depth =
		   puff_depth * (   averaged_over / static_cast<double>(new_avg)) +
		du.puff_depth * (du.averaged_over / static_cast<double>(new_avg));

	puff_edges =
		   puff_edges * (   averaged_over / static_cast<double>(new_avg)) +
		du.puff_edges * (du.averaged_over / static_cast<double>(new_avg));

	puff_sectors =
		   puff_sectors * (   averaged_over / static_cast<double>(new_avg)) +
		du.puff_sectors * (du.averaged_over / static_cast<double>(new_avg));

	averaged_over = new_avg;
}

void from_json(const json & j, data_unit & du) {
	du.averaged_over = j["averaged_over"];
	du.target_graph_ratio = j["target_graph_ratio"];
	du.graph_ratio = j["graph_ratio"];
	du.graph_size = j["graph_size"];
	du.max_puff_depth = j["max_puff_depth"];
	du.puff_depth = j["puff_depth"];
	du.puff_edges = j["puff_edges"];
	du.puff_sectors = j["puff_sectors"];
}

void to_json(json & j, const data_unit & du) {
	j["averaged_over"] = du.averaged_over;
	j["target_graph_ratio"] = du.target_graph_ratio;
	j["graph_ratio"] = du.graph_ratio;
	j["graph_size"] = du.graph_size;
	j["max_puff_depth"] = du.max_puff_depth;
	j["puff_depth"] = du.puff_depth;
	j["puff_edges"] = du.puff_edges;
	j["puff_sectors"] = du.puff_sectors;
}