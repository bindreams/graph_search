#pragma once
#include "sector.hpp"
#include "util/enviroment.hpp"

template<class T>
inline graph_match sector<T>::contains(const sector& other) const {
	//Sectors must be of one size
	GS_ASSERT(nodes.size() == other.nodes.size());

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

#include <iostream>

template<class T>
inline sector<T>& sector<T>::join_children(const sector<T>& other) {
	GS_ASSERT(nodes == other.nodes);

	children.insert(other.children.begin(), other.children.end());

	return *this;
}

template<class T>
inline std::size_t sector<T>::size_in_bytes() const {
	return
		nodes.size() * sizeof(node<T>*) +
		children.size() * sizeof(sector<T>*);
}

template<class T>
inline sector<T>::sector(const node<T>& nd) :
	nodes({&nd}) {
}

template<class T>
inline sector<T>::sector(const sector<T>& sec, const node<T>& nd) :
	nodes(sec.nodes),
	children({&sec}) {
	nodes.insert(&nd);
	// Assert that this node was not already in the sector
	GS_ASSERT(nodes.size() == sec.nodes.size() + 1);
}

template<class T>
inline sector<T>::sector(const sector& child1, const sector& child2) {
	// Children must have identical node_groups except for one node.
	GS_ASSERT(child1.nodes.size() == child2.nodes.size());

	nodes.insert(child1.nodes.begin(), child1.nodes.end());
	nodes.insert(child2.nodes.begin(), child2.nodes.end());

	// Children must have identical node_groups except for one node.
	GS_ASSERT(nodes.size() == child1.nodes.size() + 1);

	children.insert(&child1);
	children.insert(&child2);
}

template<class T> inline bool operator==(const sector<T>& lhs, const sector<T>& rhs) { return lhs.nodes == rhs.nodes && lhs.children == rhs.children; }
template<class T> inline bool operator!=(const sector<T>& lhs, const sector<T>& rhs) { return !operator==(lhs, rhs); }
template<class T> inline bool operator< (const sector<T>& lhs, const sector<T>& rhs) { return lhs.nodes < rhs.nodes; }
template<class T> inline bool operator> (const sector<T>& lhs, const sector<T>& rhs) { return  operator< (rhs, lhs); }
template<class T> inline bool operator<=(const sector<T>& lhs, const sector<T>& rhs) { return !operator> (lhs, rhs); }
template<class T> inline bool operator>=(const sector<T>& lhs, const sector<T>& rhs) { return !operator< (lhs, rhs); }

template <class T>
std::ostream& operator<<(std::ostream& os, const sector<T>& obj) {
	os << "{";

	for (auto&& i : obj.nodes) {
		os << *i;
	}

	os << "}";

	return os;
}
