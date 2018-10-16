#pragma once
#include <algorithm>
#include <set>

#include <boost/container/flat_set.hpp>
using boost::container::flat_set;

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
			node_value_order<T>());
	}

	inline bool operator() (const sector<T>* const& lhs, const sector<T>* const& rhs) const {
		return operator()(*lhs, *rhs);
	}
};

template <class T>
class sector {
public:
	flat_set<const node<T>*, node_value_order<T>> nodes;
	flat_set<const sector<T>*, sector_lexicographical_order<T>> children;

	graph_match contains(const sector& other) const;
	sector& join_children(const sector& other);
	size_t size_in_bytes() const;

	constexpr sector() = default;
	sector(const sector<T>& other) = default;
	sector(const node<T>* nd);
	sector(const sector<T>* sec, const node<T>* nd);

	std::set<sector<T>> expand() const;
};

template<class T>
inline graph_match sector<T>::contains(const sector& other) const {
	if (nodes.size() != other.nodes.size()) return {}; //Exit 1: sectors are not of of one size

	if (nodes.size() == 1) {
		if ((*nodes.begin())->value() !=
			(*other.nodes.begin())->value()) return {}; //Exit 2a: Values are different

		//std::cout << "Sectors " << *this << " and " << other << " conform" << std::endl;
		//std::cin.get();

		graph_match rslt;
		rslt.add((*nodes.begin())->id(), (*other.nodes.begin())->id());
		//std::cout << "And return this: " << rslt << std::endl;
		return rslt;
	}
	else {
		if (!std::equal(nodes.begin(), nodes.end(), other.nodes.begin(),
			node_value_equal<T>())) return {}; //Exit 2b: Values are different

		//std::cout << "Sectors " << *this << " and " << other << " conform so far" << std::endl;
		//std::cin.get();

		graph_match rslt;
		//In the following loop, i represents this->children, and j - other.children
		//For each j, there must be an equivalent i. So if i reaches end() before j
		//return failure
		for (auto i = children.begin(), j = other.children.begin(); j != other.children.end(); i++, j++) {
			if (i == children.end()) {
				//std::cout << "Sectors " << *this << " and " << other << " failed on children! 3a" << std::endl;
				return {};
			}; //Exit 3a: Children not conformant
			
			graph_match temp;
			while (!(temp = (*i)->contains(**j))) {
				i++;
				if (i == children.end()) {
					//std::cout << "Sectors " << *this << " and " << other << " failed on children! 3b" << std::endl;
					return {};
				}//Exit 3a: Children not conformant
			}
			rslt += temp;
		}

		//std::cout << "Sectors " << *this << " and " << other << " fully conform! 0" << std::endl;
		//std::cout << "And return this: " << rslt << std::endl;
		return rslt; //Exit 0: Success
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
	nodes.insert(nd);
}

// Searches for all possible expansions by 1 node,
// and returns a vector of them.
template<class T>
inline std::set<sector<T>> sector<T>::expand() const {
	std::set<sector<T>> rslt;

	//For every node in current sector...
	for (auto&& i : nodes) {
		//...check its connected nodes...
		for (auto&& j : i->edges()) {
			//...if this node is not in the sector, add
			if (nodes.find(j) == nodes.end()) {
				rslt.emplace(this, j);
			}
		}
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

	for (auto&& i : obj.nodes) {
		os << *i;
	}

	os << "}";

	return os;
}
