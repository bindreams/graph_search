#pragma once
#include <algorithm>
#include <limits>
#include <set>
#include <vector>
#include <list>
#include <deque>
#include <thread>
#include <cmath>
#include <boost/container/flat_set.hpp>

#include "sector.hpp"
#include "graph.hpp"
#include "puff_info.hpp"
#include "level_builder.hpp"
using boost::container::flat_set;

template <class T>
class puff {
	//Deque contains lists of sectors of the same size
	std::deque<
		std::list<sector<T>>> sectors;

	bool grow();
public:
	mutable puff_info info;

	size_t depth() const;

	std::set<graph_match> contains(const puff<T>& other) const;

	size_t count_edges() const;
	size_t count_sectors() const;
	size_t size_in_bytes() const;

	puff() = default;
	puff(const graph<T>& gr, size_t max_depth = std::numeric_limits<std::size_t>::max());

	template <class T_>
	friend std::ostream& operator<<(std::ostream& os, const puff<T_>& obj);
};

template<class T>
inline puff<T>::puff(const graph<T>& gr, size_t max_depth) {
	if (max_depth == 0) throw std::invalid_argument("max_depth must be at least 1");

	// Build level 1
	sectors.emplace_back();
	for (auto& i : gr) {
		sectors.front().emplace_back(&i);
	}

	// Build level 2, if needed
	if (max_depth > 1) {
		// Create 2 vectors, each holding same sectors but produced from
		// different children. In the end, two vectors will be merged.
		std::vector<sector<T>> new_level1;
		std::vector<sector<T>> new_level2;

		// For each sector in first level
		for (auto& i : sectors.front()) {
			// Check edges
			const node<T>& only_node = (**i.nodes.begin());
			for (auto& edge : only_node.edges()) {
				// Based on edges id emplace in one of vectors
				if (edge->id() > only_node.id())
					new_level1.emplace_back(&i, edge);
				else
					new_level2.emplace_back(&i, edge);
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
	for (size_t level = 2; level < max_depth; level++) {
		//std::cout << "level " << level << " growth" << std::endl;
		level_builder<T> lb;
		std::size_t block_size = std::ceil(
			static_cast<double>(sectors.back().size()) /
			std::thread::hardware_concurrency());
		//std::cout << "Size: " << sectors.back().size() << "; block: " << block_size << std::endl;
		if (!lb.build(sectors.back(), block_size)) break;

		// Insert new level in the end
		sectors.emplace(sectors.end(),
			lb.result().begin(), 
			lb.result().end());
	}

	//std::cout << "Built a puff (max_depth: " << max_depth << ", levels: " << sectors.size() << ", depth at back(): " << sectors.back().size() << "): " << *this;
}

template<class T>
inline bool puff<T>::grow() {

}

template<class T>
inline size_t puff<T>::depth() const {
	return sectors.size();
}

template<class T>
inline std::set<graph_match> puff<T>::contains(const puff<T>& other) const {
	info.async_calls_contains_ = 0;

	if (other.depth() > depth()) {
		return {};
	}

	std::set<graph_match> rslt;

	for (auto&& i : other.sectors[other.depth() - 1]) {
		std::set<graph_match> matches_of_one;

		std::vector<std::future<graph_match>> matches;
		for (auto&& j : sectors[other.depth() - 1]) {
			info.async_calls_contains_++;
			matches.push_back(std::move(std::async(std::launch::async, &sector<T>::contains, &j, std::cref(i))));
		}

		for (auto&& j : matches) {
			graph_match match (std::move(j.get()));
			if (match) matches_of_one.emplace(match);
		}

		if (matches_of_one.empty()) return {};
		rslt = merge(rslt, matches_of_one);
	}

	return rslt;
}

template<class T>
inline size_t puff<T>::count_edges() const {
	size_t rslt = 0;

	for (auto i : sectors) {
		for (auto j : i) {
			rslt += j.children.size();
		}
	}

	return rslt;
}

template<class T>
inline size_t puff<T>::count_sectors() const {
	size_t rslt = 0;
	
	for (auto i : sectors) rslt += i.size();

	return rslt;
}

template<class T>
inline size_t puff<T>::size_in_bytes() const {
	size_t rslt = 0;

	for (auto&& i : sectors) {
		for (auto&& j : i) {
			rslt += j.size_in_bytes();
		}
	}

	return rslt;
}

template <class T>
std::ostream& operator<<(std::ostream& os, const puff<T>& obj) {
	os << "{" << std::endl;

	for (size_t level = obj.depth()-1; level != 0; level--) {
		std::cout << "    level " << level << " {" << std::endl;

		for (auto&& i : obj.sectors[level]) {
			os << "        " << i << " <= ";
			for (auto&& j : i.children) os << *j << " ";
			os << std::endl;
		}

		std::cout << "    }" << std::endl;
	}

	os << "}";

	return os;
}