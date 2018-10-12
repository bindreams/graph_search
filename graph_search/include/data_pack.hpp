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

template<class T>
inline void data_pack::append(const graph<T>& gr, std::size_t max_depth, double target_ratio) {
	data_unit du(gr, max_depth, target_ratio);

	auto got = std::lower_bound(data.begin(), data.end(), du, data_unit_lex_order());

	if (got == data.end() || data_unit_lex_order()(du, *got)) {
		// No similar data_unit in vector
		data.insert(got, du);
	}
	else {
		// Merge with similar
		got->merge(du);
	}
}
