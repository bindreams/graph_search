#pragma once
#include <cstddef>
#include <utility>

namespace zh {
namespace proxy {

struct dereference {
	template <class PtrLikeType>
	decltype(auto) operator()(PtrLikeType&& ptr) const {
		return *ptr;
	}
};

template <std::size_t index>
struct get_element {
	template <class PairType>
	decltype(auto) operator()(PairType&& pair_obj) const {
		return std::get<index>(pair_obj);
	}
};

using get_first = get_element<0>;
using get_second = get_element<1>;

struct remove_const {
	// Apparently, std::remove_const_t<const T&> would not actually remove
	// const, because it does not count that const as being top-level.
	// Workaround: std::remove_const_t<const T>&
	template <class T>
	std::remove_const_t<T>& operator()(T& obj) const {
		return const_cast<std::remove_const_t<T>&>(obj);
	}
};

struct add_const {
	template <class T>
	T const& operator()(T& obj) const {
		return obj;
	}
};

} // namespace proxy
} // namespace zh