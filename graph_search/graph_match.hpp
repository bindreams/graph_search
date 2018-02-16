#pragma once
#include <map>
#include <set>
#include <utility>
#include "print.hpp"

class graph_match {
public:
	std::map<const void*, const void*> matches;

	template <typename T>
	void add(const T* first, const T* second);

	graph_match& operator+=(const graph_match& rhs);

	explicit operator bool();
};

inline graph_match::operator bool() {
	return !matches.empty();
}

inline bool operator==(const graph_match& lhs, const graph_match& rhs) {
	return lhs.matches == rhs.matches;
}

inline bool operator!=(const graph_match& lhs, const graph_match& rhs) {
	return !operator==(lhs, rhs);
}

inline bool operator< (const graph_match& lhs, const graph_match& rhs) {
	return lhs.matches < rhs.matches;
}

inline graph_match& graph_match::operator+=(const graph_match & rhs) {
	matches.insert(rhs.matches.begin(), rhs.matches.end());

	return *this;
}

inline graph_match operator+(graph_match lhs, const graph_match& rhs) {
	lhs += rhs;
	return lhs;
}

std::ostream& operator<<(std::ostream& os, const graph_match& obj) {
	os << "{" << std::endl;

	for (auto i : obj.matches) {
		os << "    {" << tr(i.first) << " - " << tr(i.second) << "}" << std::endl;
	}

	os << "}";

	return os;
}

template<typename T>
inline void graph_match::add(const T* first, const T* second) {
	matches.insert(std::make_pair(static_cast<const void*>(first), static_cast<const void*>(second)));
}

inline std::set<graph_match> merge(const std::set<graph_match>& v1, const std::set<graph_match>& v2) {
	if (v1.empty()) return v2;
	if (v2.empty()) return v1;

	std::set<graph_match> rslt;

	//Merge
	for (auto&& i : v1) {
		for (auto&& j : v2) {
			rslt.emplace(i + j);
		}
	}

	return rslt;
}