#pragma once
#include <boost/container/flat_set.hpp>
#include "node.hpp"
#include "functors/node_functors.hpp"
using boost::container::flat_set;

template <class T>
class node_group : public flat_set<const node<T>*, node_value_order<T>> {
private:
	using base = flat_set<const node<T>*, node_value_order<T>>;
public:
	using base::base;
};