#pragma once

template <class T>
class node;

template<class T>
struct node_id_equal {
	inline bool operator() (const node<T>& lhs, const node<T>& rhs) const noexcept;
};

struct access_value {
	template <class T>
	decltype(auto) operator()(T&& obj) const noexcept;
};

#include "node_functors.inl"