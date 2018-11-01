#pragma once
#include <set>
#include <boost/container/flat_set.hpp>
#include "node.hpp"
using boost::container::flat_set;

template <class T>
class node_group : public flat_set<const node<T>*, node_value_order<T>> {
private:
	using base = flat_set<const node<T>*, node_value_order<T>>;
public:
	using base::base;

	// Return a vector of all possible node_groups
	// that are missing one element from *this.
	std::vector<node_group> except_1() const;
};

namespace std {

template <class T>
struct hash<node_group<T>> {
	std::size_t operator()(const node_group<T>& obj) const {
		std::size_t rslt = 0;

		for (auto& i : obj) {
			rslt ^= hash<const node<T>*>()(i);
		}

		return rslt;
	}
};

}

template<class T>
inline std::vector<node_group<T>> node_group<T>::except_1() const {
	std::vector<node_group> rslt;
	rslt.reserve(this->size());

	for (auto not_me = this->begin(); not_me != this->end(); ++not_me) {
		// Emplace with elements left of excluded
		rslt.emplace_back(this->begin(), not_me);
		// Insert element right of excluded
		rslt.back().insert(std::next(not_me), this->end());
	}

	return rslt;
}
