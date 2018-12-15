#pragma once
#include <cstddef>
#include <utility>

namespace zh {

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

} // namespace zh