#pragma once
#include <iostream>
#include <fstream>
#include <filesystem>
#include "report/report.hpp"
#include "graph_manip.hpp"
#include "generator.hpp"
#include "report/data_pack.hpp"
namespace fs = std::filesystem;

template <class Range1,	class Range2, class Range3>
inline void report(const std::string& save_path,
		const Range1& sizes,
		const Range2& depths,
		const Range3& ratios,
		int attempts) {

	data_pack data;
	// If file previously existed, append data to existing data
	if (fs::exists(save_path)) {
		std::ifstream ifs(save_path);
		json j;

		ifs >> j;
		data = j;
	}

	for (double p : ratios) {
		for (int k : depths) {
			for (int n : sizes) {
				std::cout << "Computing for "
					<< "target ratio = " << p << ", "
					<< "max depth = " << k << ", "
					<< "size = " << n << std::endl;
				std::cout << "Average over " << attempts << ": ";

				for (int attempt = 1; attempt <= attempts; attempt++) {
					std::cout << attempt << " ";

					graph<int> gr;
					mutate(gr, n, p, test_gen());

					data.append(gr, k, p);
					data.dump_to_file("_temp_dump.json");
				}

				std::cout << std::endl;
			}
		}
	}

	data.dump_to_file(save_path);
	fs::remove("_temp_dump.json");
}