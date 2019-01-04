#pragma once
#include <type_traits>
#include "remove_all_pointers.hpp"

namespace zh {

template <class F>
struct is_function_like
	: public std::is_function<remove_all_pointers_t<std::remove_reference_t<F>>> {
};

template <class F>
constexpr bool is_function_like_v = is_function_like<F>::value;

} // namespace zh