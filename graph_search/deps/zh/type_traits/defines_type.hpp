#pragma once
#include <type_traits>
#include "enable_if_defined.hpp"

namespace zh {

template <class T, class Enable = void>
struct defines_type : std::false_type {
};

template <class T>
struct defines_type<T,
	enable_if_defined_t<typename T::type>> : std::true_type {
};

template <class T>
constexpr bool defines_type_v = defines_type<T>::value;

} // namespace zh