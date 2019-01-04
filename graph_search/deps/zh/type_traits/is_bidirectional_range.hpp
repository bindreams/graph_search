#pragma once
#include <type_traits>
#include <utility>
#include <iterator>
#include "enable_if_defined.hpp"

namespace zh {

template <class T, 
	class Enable1 = void, 
	class Enable2 = void,
	class Enable3 = void,
	class Enable4 = void>
struct is_bidirectional_range : std::false_type {
};

template <class T>
struct is_bidirectional_range<T,
	enable_if_defined_t<decltype(std::begin(std::declval<T>()))>,
	enable_if_defined_t<decltype(std::end(std::declval<T>()))>,
	enable_if_defined_t<decltype(std::rbegin(std::declval<T>()))>,
	enable_if_defined_t<decltype(std::rend(std::declval<T>()))>> : std::true_type {
};

template <class T>
constexpr bool is_bidirectional_range_v = is_bidirectional_range<T>::value;

} // namespace zh