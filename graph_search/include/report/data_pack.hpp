#pragma once
#include <vector>
#include "data_unit.hpp"
#include "data_unit_lex_order.hpp"

class data_pack {
private:
	std::vector<data_unit> data;

public:
	data_unit& at(
		std::size_t graph_size,
		std::size_t max_puff_depth,
		double graph_ratio);

	template <class T>
	void append(const graph<T>& gr, std::size_t max_depth, double target_ratio = -1);

	std::string dump();
	void dump_to_file(const std::string& file);

	friend void from_json(const json& j, data_pack& du);
	friend void to_json(json& j, const data_pack& du);
};

#include "inline/data_pack.inl"
