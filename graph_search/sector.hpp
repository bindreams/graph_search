#pragma once
#include <algorithm>
#include <set>
#include "node.hpp"
#include "graph_match.hpp"

template <class T>
class sector;

template<class T>
struct sector_lexicographical_order {
	inline bool operator() (sector<T> const& lhs, sector<T> const& rhs) const {
		return std::lexicographical_compare(
			lhs.nodes.begin(), lhs.nodes.end(),
			rhs.nodes.begin(), rhs.nodes.end(),
			node_numerical_order<T>());
	}

	inline bool operator() (const sector<T>* const& lhs, const sector<T>* const& rhs) const {
		return operator()(*lhs, *rhs);
	}
};

template <class T>
class sector {
public:
	std::set<const node<T>*, node_numerical_order<T>> nodes;
	std::set<const sector<T>*, sector_lexicographical_order<T>> children;

	graph_match contains(const sector& other) const;
	sector& join_children(const sector& other);
	size_t size_in_bytes() const;

	constexpr sector() = default;
	sector(const sector<T>& other) = default;
	sector(const node<T>* nd);
	sector(const sector<T>* sec, const node<T>* nd);

	std::set<sector<T>> expand() const;
	std::set<sector<T>> try_expand() const;
};

template<class T>
inline graph_match sector<T>::contains(const sector& other) const {
	if (nodes.size() != other.nodes.size()) return {};

	if (nodes.size() == 1) {
		if ((*nodes.begin())->value !=
			(*other.nodes.begin())->value) return {};

		graph_match rslt;
		rslt.add((*nodes.begin())->get_id(), (*other.nodes.begin())->get_id());
		return rslt;
	}
	else {
		if (!std::equal(nodes.begin(), nodes.end(), other.nodes.begin(),
			node_equal_values<T>())) return {};

		graph_match rslt;
		for (auto i = other.children.begin(), j = children.begin(); i != other.children.end(); i++, j++) {
			graph_match temp;
			while (!(temp = (*j)->contains(**i))) {
				j++;
				if (j == children.end()) return {};
			}
			rslt += temp;
		}

		return rslt;
	}
}

template<class T>
inline sector<T>& sector<T>::join_children(const sector<T>& other) {
	if (nodes != other.nodes) throw std::runtime_error("attempting to join different sectors");

	children.insert(other.children.begin(), other.children.end());

	return *this;
}

template<class T>
inline size_t sector<T>::size_in_bytes() const {
	return
		nodes.size() * sizeof(node<T>*) +
		children.size() * sizeof(sector<T>*);
}

template<class T>
inline sector<T>::sector(const node<T>* nd) :
	nodes({nd}) {
}

template<class T>
inline sector<T>::sector(const sector<T>* sec, const node<T>* nd) :
	nodes(sec->nodes),
	children({sec}) {
	nodes.emplace(nd);
}

template<class T>
inline std::set<sector<T>> sector<T>::expand() const {
	std::set<sector<T>> rslt;

	//For every node in current sector...
	for (auto&& i : nodes) {
		//...check its connected nodes...
		for (auto&& j : i->get_edges()) {
			//...if this node is not in the sector, add
			if (nodes.find(j) == nodes.end()) {
				rslt.emplace(this, j);
			}
		}
	}

	return rslt;
}

template<class T>
inline std::set<sector<T>> sector<T>::try_expand() const {
	//Tries to expand
	std::set<sector<T>> rslt = expand();

	if (rslt.empty()) {
		//if fails, return copy of this with this as child
		sector<T> copy_of_me(*this);
		copy_of_me.children = {this};

		rslt.emplace(std::move(copy_of_me));
	}
	
	return rslt;
}

template<typename T> inline bool operator==(const sector<T>& lhs, const sector<T>& rhs) { return lhs.nodes == rhs.nodes && lhs.children == rhs.children; }
template<typename T> inline bool operator!=(const sector<T>& lhs, const sector<T>& rhs) { return !operator==(lhs, rhs); }
template<typename T> inline bool operator< (const sector<T>& lhs, const sector<T>& rhs) { return lhs.nodes < rhs.nodes; }
template<typename T> inline bool operator> (const sector<T>& lhs, const sector<T>& rhs) { return  operator< (rhs, lhs); }
template<typename T> inline bool operator<=(const sector<T>& lhs, const sector<T>& rhs) { return !operator> (lhs, rhs); }
template<typename T> inline bool operator>=(const sector<T>& lhs, const sector<T>& rhs) { return !operator< (lhs, rhs); }

template <class T>
std::ostream& operator<<(std::ostream& os, const sector<T>& obj) {
	os << "{";
	for (auto&& i : obj.nodes) os << i->value;

	os << "}";

	return os;
}
