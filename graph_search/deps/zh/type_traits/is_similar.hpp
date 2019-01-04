#pragma once
#include "is_same.hpp"

namespace zh {

template <class T, class U, class...Ts>
struct is_similar
	: is_same<
		std::decay_t<T>, 
		std::decay_t<U>,
		std::decay_t<Ts>...> {
};

template <class T, class U, class... Ts>
constexpr bool is_similar_v = is_similar<T, U, Ts...>::value;

} // namespace zh