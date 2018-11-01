#pragma once
#include "sector.hpp"

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

template<class T>
inline sector<T>::sector(const sector& child1, const sector& child2) {
	nodes.insert(child1.nodes.begin(), child1.nodes.end());
	nodes.insert(child2.nodes.begin(), child2.nodes.end());

	children.insert(&child1);
	children.insert(&child2);
}

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
