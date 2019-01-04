#pragma once
#include <type_traits>
#include "is_more_cv.hpp"

namespace zh {

// is_weaker_qualified - checks that a type T is a more weakly qualified
// version of U

template <class T, class U>
struct is_weaker_qualified
	: std::integral_constant<bool,
	(std::is_same_v<
		std::remove_cv_t<T>,
		std::remove_cv_t<U>> &&
		is_less_cv_v<T, U>)> {
};

template <class T, class U>
constexpr bool is_weaker_qualified_v = is_weaker_qualified<T, U>::value;

}