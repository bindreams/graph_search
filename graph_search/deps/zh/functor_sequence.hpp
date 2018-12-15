#pragma once

namespace zh {
namespace detail {

// functor_sequence_impl is the implementation of functor sequence
// accepts void parameter as a marker of end
template <class Functor1, class... Functors>
struct functor_sequence_impl {
	template <class T>
	decltype(auto) operator()(T&& obj) const {
		return functor_sequence_impl<Functors...>()(
			Functor1()(std::forward<T>(obj))
			);
	}
};

template <class Functor>
struct functor_sequence_impl<Functor, void> {
	template <class T>
	decltype(auto) operator()(T&& obj) const {
		return Functor()(std::forward<T>(obj));
	}
};

} // namespace detail

// functor_sequence is a functor, that applies unary functors to it in sequence
template <class... Functors>
using functor_sequence = detail::functor_sequence_impl<Functors..., void>;

} // namespace zh