#pragma once
#include "node_functors.hpp"
#include "node.hpp"

template<class T>
inline bool node_id_equal<T>::operator()(const node<T>& lhs, const node<T>& rhs) const noexcept {
	return lhs.id() == rhs.id();
}

template <class T>
decltype(auto) access_value::operator()(T&& obj) const noexcept {
	return obj.value();
}