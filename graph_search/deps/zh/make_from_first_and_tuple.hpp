#pragma once
#include <tuple>

// Make from one element and a tuple. Internal, used in container_view_impl.hpp
namespace zh {
namespace detail {

template <class T, class First, class Tuple, std::size_t... I>
constexpr T make_from_first_and_tuple_impl(First&& first, Tuple&& t, std::index_sequence<I...>) {
	return T(std::forward<First>(first), std::get<I>(std::forward<Tuple>(t))...);
}

template <class T, class First, class Tuple>
constexpr T make_from_first_and_tuple(First&& first, Tuple&& t) {
	return detail::make_from_first_and_tuple_impl<T>(
		std::forward<First>(first), 
		std::forward<Tuple>(t),
		std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
}

} // namespace detail
} // namespace zh