#pragma once
#include <string>
#include <initializer_list>

template <
	class Range1 = std::initializer_list<int>,
	class Range2 = std::initializer_list<int>,
	class Range3 = std::initializer_list<double>>
inline void report(const std::string& save_path,
		const Range1& sizes,
		const Range2& depths,
		const Range3& ratios,
		int attempts = 5);

#include "inline/report.inl"