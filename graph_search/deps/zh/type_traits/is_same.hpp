#pragma once
#include <type_traits>

namespace zh {

template<class T, class U, class... Ts>
struct is_same
	: std::conjunction<std::is_same<T, U>, std::is_same<T, Ts>...> {
};

template <class T, class U, class... Ts>
constexpr bool is_same_v = is_same<T, U, Ts...>::value;

} // namespace zh