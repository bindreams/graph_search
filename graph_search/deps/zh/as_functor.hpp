#pragma once
#include "type_traits/is_function_like.hpp"

namespace zh {

namespace detail {

// A representation of a function as a functor. Enables that function to be
// derived from to take advantage of EBO.
template <auto function>
struct as_functor_impl {
	template <class... Args>
	constexpr decltype(auto) operator()(Args&&... args) const {
		return function(std::forward<Args>(args)...);
	}
};

} // namespace detail

template <class Function, class = 
	std::enable_if_t<is_function_like_v<Function>>>
constexpr decltype(auto) to_function_ref(Function* func) {
	auto& temp = *func;
	if constexpr (std::is_pointer_v<std::remove_reference_t<decltype(temp)>>) {
		return to_function_ref(temp);
	}
	else {
		return temp;
	}
}

template <auto function>
using as_functor = detail::as_functor_impl<to_function_ref(function)>;

} // namespace zh