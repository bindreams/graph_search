#pragma once
#include "report/data_pack.hpp"

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