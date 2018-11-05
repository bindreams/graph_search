#pragma once
#include "puff.hpp"

template<class T>
inline puff<T>::puff(const graph<T>& gr, std::size_t max_depth) {
	if (max_depth == 0) throw std::invalid_argument("max_depth must be at least 1");
	sectors.reserve(std::min(max_depth, gr.size()));

	// Build level 1
	sectors.emplace_back();
	for (auto& i : gr) {
		sectors.front().emplace_back(i);
	}

	// Build level 2, if needed
	if (max_depth > 1) {
		// Create 2 vectors, each holding same sectors but produced from
		// different children. In the end, two vectors will be merged.
		std::vector<sector<T>> new_level1;
		std::vector<sector<T>> new_level2;

		// For each sector in first level
		for (auto& sector : sectors.front()) {
			// Check edges
			const node<T>& only_node = (**sector.nodes.begin());
			for (auto& edge : only_node.edges()) {
				// Based on edges id emplace in one of vectors
				if (edge->id() > only_node.id())
					new_level1.emplace_back(sector, *edge);
				else
					new_level2.emplace_back(sector, *edge);
			}
		}

		// If vectors are empty, no level to build
		if (new_level1.empty()) return;

		// Sort and merge sectors
		std::sort(new_level1.begin(), new_level1.end());
		std::sort(new_level2.begin(), new_level2.end());

		for (std::size_t i = 0; i < new_level1.size(); i++) {
			new_level1[i].join_children(new_level2[i]);
		}

		sectors.emplace_back(new_level1.begin(), new_level1.end());
	}

	// Build remaining levels using already built ones
	level_builder<T> lb;
	for (std::size_t level = 2; level < max_depth; level++) {
		//std::cout << "level " << level << " growth" << std::endl;
		if (!lb.build(sectors.back())) break;

		// Insert new level in the end
		sectors.emplace(sectors.end(),
			lb.result().begin(),
			lb.result().end());
	}

	//std::cout << "Built a puff (max_depth: " << max_depth << ", levels: " << sectors.size() << ", depth at back(): " << sectors.back().size() << "): " << *this;
}

template<class T>
inline std::size_t puff<T>::depth() const {
	return sectors.size();
}

template<class T>
inline std::size_t puff<T>::count_edges() const {
	std::size_t rslt = 0;

	for (auto i : sectors) {
		for (auto j : i) {
			rslt += j.children.size();
		}
	}

	return rslt;
}

template<class T>
inline std::size_t puff<T>::count_sectors() const {
	std::size_t rslt = 0;

	for (auto i : sectors) rslt += i.size();

	return rslt;
}

template<class T>
inline std::size_t puff<T>::size_in_bytes() const {
	std::size_t rslt = 0;

	for (auto&& i : sectors) {
		for (auto&& j : i) {
			rslt += j.size_in_bytes();
		}
	}

	return rslt;
}

template<class T>
inline std::set<graph_match> puff<T>::contains(const puff<T>& other) const {
	if (other.depth() > depth()) {
		return {};
	}

	std::set<graph_match> rslt;

	for (auto&& i : other.sectors[other.depth() - 1]) {
		std::set<graph_match> matches_of_one;

		std::vector<std::future<graph_match>> matches;
		for (auto&& j : sectors[other.depth() - 1]) {
			matches.push_back(std::move(std::async(std::launch::async, &sector<T>::contains, &j, std::cref(i))));
#ifdef GS_COLLECT_STATS
			stats.add_async_call_contains();
#endif
		}

		for (auto&& j : matches) {
			graph_match match(std::move(j.get()));
			if (match) matches_of_one.emplace(match);
		}

		if (matches_of_one.empty()) return {};
		rslt = merge(rslt, matches_of_one);
	}

	return rslt;
}

template<class T>
inline const typename puff<T>::level_container&
puff<T>::operator[](std::size_t idx) const noexcept {
	return sectors[idx];
}

template <class T>
std::ostream& operator<<(std::ostream& os, const puff<T>& obj) {
	os << "{" << std::endl;

	for (std::size_t level = obj.depth() - 1; level != 0; level--) {
		std::cout << "    level " << level << " {" << std::endl;

		for (const auto& i : obj[level]) {
			os << "        " << i << " <= ";
			for (const auto& j : i.children) os << *j << " ";
			os << std::endl;
		}

		std::cout << "    }" << std::endl;
	}

	os << "}";

	return os;
}