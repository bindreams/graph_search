#pragma once
#include <algorithm>
#include <limits>
#include <set>
#include <vector>
#include <list>
#include <deque>
#include "sector.hpp"
#include "graph_impl.hpp"

template <class T>
class puff {
	//Inner vector contains all sectors of the same size
	//Outer vector contains all sectors
	std::deque<
		std::list<sector<T>>> sectors;

public:
	size_t depth() const;

	std::set<graph_match> contains(const puff<T>& other) const;

	size_t size() const;
	size_t size_in_bytes() const;

	puff(const graph_impl<T>& gr, size_t max_depth = std::numeric_limits<std::size_t>::max());

	template <class T_>
	friend std::ostream& operator<<(std::ostream& os, const puff<T_>& obj);
};

template<class T>
inline puff<T>::puff(const graph_impl<T>& gr, size_t max_depth) {
	if (max_depth != 0) {

		sectors.push_back({});
		for (auto&& i : gr.nodes) {
			sectors.front().emplace_back(i.get());
		}

		for (size_t level = 1; level < max_depth; level++) {
			//Expand all elements of previous level by 1 and insert
			std::list<sector<T>> new_level;

			bool expanded = false;

			for (auto&& i : sectors.back()) {
				auto expanded_sectors = i.expand();
				if (!expanded_sectors.empty()) {
					expanded = true;
					new_level.insert(
						new_level.end(), expanded_sectors.begin(), expanded_sectors.end());
				}
			}
			if (!expanded) break;

			//Trim: join identical sectors' children
			new_level.sort(sector_lexicographical_order<T>());

			for (auto control = new_level.begin(); control != new_level.end(); control++) {
				for (auto check = std::next(control); check != new_level.end() && control->nodes == check->nodes;) {
					control->join_children(*check);
					check = new_level.erase(check);
				}
			}

			sectors.emplace_back(std::move(new_level));
		}
	}
}

template<class T>
inline size_t puff<T>::depth() const {
	return sectors.size();
}

template<class T>
inline std::set<graph_match> puff<T>::contains(const puff<T>& other) const {
	if (other.depth() > depth()) {
		return {};
	}

	std::set<graph_match> rslt;

	for (auto&& i : other.sectors[other.depth() - 1]) {
		std::set<graph_match> matches_of_one;
		for (auto&& j : sectors[other.depth() - 1]) {
			graph_match match (j.contains(i));
			if (match) matches_of_one.emplace(match);
		}
		if (matches_of_one.empty()) return {};
		rslt = merge(rslt, matches_of_one);
	}

	return rslt;
}

template<class T>
inline size_t puff<T>::size() const {
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

	size_t level = obj.depth();
	do {
		level--;
		std::cout << "    level " << level << ":" << std::endl;
		for (auto&& i : obj.sectors[level]) {
			os << "        " << i << " -> ";
			for (auto&& j : i.children) os << *j << " ";
			os << std::endl;
		}
	} while (level != 0);

	os << "}";

	return os;
}