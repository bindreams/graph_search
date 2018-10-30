#pragma once
#include <type_traits>
#include <iterator>
#include <ratio>

namespace zh {

// Internal ===================================================================
template<class T, class R = void>
struct enable_if_defined {
	using type = void;
};

template <class T, class R = void>
using enable_if_defined_t = typename enable_if_defined<T, R>::type;

// defines_type ---------------------------------------------------------------
template <class T, class Enable = void>
struct defines_type : std::false_type {
};

template <class T>
struct defines_type<T,
	enable_if_defined_t<typename T::type>> : std::true_type {
};

template <class T>
constexpr bool defines_type_v = defines_type<T>::value;

// is_range ------------------------------------------------------------------
template <class T, class Enable1 = void, class Enable2 = void>
struct is_range : std::false_type {
};

template <class T>
struct is_range<T,
	enable_if_defined_t<decltype(std::begin(std::declval<T>()))>,
	enable_if_defined_t<decltype(std::end(std::declval<T>()))>> : std::true_type {
};

template <class T>
constexpr bool is_range_v = is_range<T>::value;

// is_ratio ------------------------------------------------------------------
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

}