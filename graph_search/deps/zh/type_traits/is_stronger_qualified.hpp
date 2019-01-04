#pragma once
#include <type_traits>
#include "is_more_cv.hpp"

namespace zh {

// is_stronger_qualified - checks that a type T is a more strongly qualified
// version of U

template <class T, class U>
struct is_stronger_qualified
	: std::integral_constant<bool,
	(std::is_same_v<
		std::remove_cv_t<T>,
		std::remove_cv_t<U>> &&
		is_more_cv_v<T, U>)> {
};

template <class T, class U>
constexpr bool is_stronger_qualified_v = is_stronger_qualified<T, U>::value;

}