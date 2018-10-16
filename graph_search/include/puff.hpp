#pragma once
#include <algorithm>
#include <limits>
#include <set>
#include <vector>
#include <list>
#include <deque>
#include <future>

#include "sector.hpp"
#include "graph.hpp"
#include "puff_info.hpp"

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

	sectors.push_back({});
	for (auto&& i : gr) {
		sectors.front().emplace_back(&i);
	}

	for (size_t level = 1; level < max_depth; level++) {
		//std::cout << "level " << level << " growth" << std::endl;
		if (!grow()) break;
	}

	//std::cout << "Built a puff (max_depth: " << max_depth << ", levels: " << sectors.size() << ", depth at back(): " << sectors.back().size() << "): " << *this;
}

template<class T>
inline bool puff<T>::grow() {
	if (sectors.empty()) throw std::runtime_error("attemted to grow an empty puff");

	//Expand all elements of previous level by 1 and insert
	std::list<sector<T>> new_level;

	std::vector<std::future<std::set<sector<T>>>> expanded_sectors;
	for (auto&& i : sectors.back()) {
		info.async_calls_ctor_++;
		expanded_sectors.push_back(std::move(std::async(std::launch::async, &sector<T>::expand, &i)));
	}
	
	bool expanded = false;
	
	for (auto&& i : expanded_sectors) {
		auto temp = std::move(i.get());
		if (!temp.empty()) expanded = true;
		new_level.insert(
			new_level.end(), temp.begin(), temp.end());
	}
	
	if (!expanded) return false;
	new_level.sort(sector_lexicographical_order<T>());

	//Trim: join identical sectors' children
	for (auto control = new_level.begin(); control != new_level.end(); control++) {
		for (auto check = std::next(control); check != new_level.end() && control->nodes == check->nodes;) {
			control->join_children(*check);
			check = new_level.erase(check);
		}
	}

	sectors.emplace_back(std::move(new_level));
	return true;
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