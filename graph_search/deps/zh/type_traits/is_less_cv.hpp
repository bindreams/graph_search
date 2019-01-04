#pragma once
#include <type_traits>

namespace zh {

// is_less_cv and is_more_cv - compare constraints of two types
// const int is more cv than int
// const int is NOT more cv than volatile int
// "more" in this context means that, assuming 
// remove_cv_t<T> is same as remove_cv_t<U>,
// U can be upgraded to T without losing qualifiers.

template <class T, class U>
struct is_less_cv
	: std::integral_constant<bool, (
		std::is_const_v<T>    <= std::is_const_v<U> &&
		std::is_volatile_v<T> <= std::is_volatile_v<U>)> {
};

template <class T, class U>
constexpr bool is_less_cv_v = is_less_cv<T, U>::value;

} // namespace zh