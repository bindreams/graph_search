#pragma once
#include <algorithm>
#include <iostream>
#include <fstream>
#include "graph_manip.hpp"
#include "generator.hpp"

inline void report1(const std::string& save_path = "report.json", int attempts = 5) {
	/*
	При фиксированной глубине слойки k, 
	по оси х отложить мощность графа N (N>k),
	по оси у - множество вершин слойки (множество ребер слойки на др. графике).
	*/

	json rslt;
	auto ratios = {0.9, 1.0};
	auto depths = {5, 7, 10, 15};

	for (double p : ratios) {
		for (int k : depths) {
			for (int n = k; n <= 20; n++) {
				std::cout << std::setw(2) << std::setfill('0')
					<< "Computing for "
					<< "p = " << p << ", "
					<< "k = " << k << ", "
					<< "n = " << n << std::endl;
				
				std::vector<int> sectors;
				std::vector<int> edges;

				for (int attempt = 0; attempt < attempts; attempt++) {
					std::cout << "  Attempt " << attempt + 1 << std::endl;
					graph<int> gr;
					mutate(gr, n, p, test_gen());
					puff pf(gr, k);

					sectors.push_back(pf.count_sectors());
					edges.push_back(pf.count_edges());
				}

				json j;
				j["graph_ratio"] = p;
				j["puff_depth"] = k;
				j["graph_size"] = n;
				j["puff_sectors"] = std::accumulate(sectors.begin(), sectors.end(), 0) / sectors.size();
				j["puff_edges"] =   std::accumulate(edges.begin(),   edges.end(),   0) / edges.size();

				rslt.push_back(j);
				// Re-save every time to prevent info loss
				std::ofstream ofs(save_path);
				ofs << std::setw(4) << rslt;
				ofs.close();
			}
		}
	}
}