#pragma once
#include <type_traits>
#include <utility>
#include "enable_if_defined.hpp"

namespace zh {

template <class T, class Enable1 = void, class Enable2 = void>
struct is_ratio : std::false_type {
};

template <class T>
struct is_ratio<T,
	enable_if_defined_t<decltype(std::declval<T>().num)>,
	enable_if_defined_t<decltype(std::declval<T>().den)>> : std::true_type {
};

template <class T>
constexpr bool is_ratio_v = is_ratio<T>::value;

} // namespace zh