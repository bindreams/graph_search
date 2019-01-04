#pragma once
#include "container_view_impl_reverse.hpp"
#include "make_from_first_and_tuple.hpp"

#define TEMPLATE_CONTAINER_VIEW \
template < \
	class Container, \
	class Iterator, \
	class ConstIterator, \
	class ReverseIterator, \
	class ConstReverseIterator, \
	class... Args>
#define CONTAINER_VIEW \
container_view_impl< \
	Container, \
	Iterator, \
	ConstIterator, \
	ReverseIterator, \
	ConstReverseIterator, \
	true, \
	Args...>

namespace zh {
namespace detail {

TEMPLATE_CONTAINER_VIEW
constexpr typename CONTAINER_VIEW::reverse_iterator
CONTAINER_VIEW::rbegin() noexcept {
	return make_from_first_and_tuple<reverse_iterator>(
		base::end(), base::args());
}

TEMPLATE_CONTAINER_VIEW
constexpr typename CONTAINER_VIEW::const_reverse_iterator
CONTAINER_VIEW::rbegin() const noexcept {
	return make_from_first_and_tuple<const_reverse_iterator>(
		base::cend(), base::args());
}

TEMPLATE_CONTAINER_VIEW
constexpr typename CONTAINER_VIEW::const_reverse_iterator
CONTAINER_VIEW::crbegin() const noexcept {
	return make_from_first_and_tuple<const_reverse_iterator>(
		base::cend(), base::args());
}

TEMPLATE_CONTAINER_VIEW
constexpr typename CONTAINER_VIEW::reverse_iterator
CONTAINER_VIEW::rend() noexcept {
	return make_from_first_and_tuple<reverse_iterator>(
		base::begin(), base::args());
}

TEMPLATE_CONTAINER_VIEW
constexpr typename CONTAINER_VIEW::const_reverse_iterator
CONTAINER_VIEW::rend() const noexcept {
	return make_from_first_and_tuple<const_reverse_iterator>(
		base::cbegin(), base::args());
}

TEMPLATE_CONTAINER_VIEW
constexpr typename CONTAINER_VIEW::const_reverse_iterator
CONTAINER_VIEW::crend() const noexcept {
	return make_from_first_and_tuple<const_reverse_iterator>(
		base::cbegin(), base::args());
}

TEMPLATE_CONTAINER_VIEW
constexpr typename CONTAINER_VIEW::reference
CONTAINER_VIEW::back() {
	return *rbegin();
}

TEMPLATE_CONTAINER_VIEW
constexpr typename CONTAINER_VIEW::const_reference
CONTAINER_VIEW::back() const {
	return *crbegin();
}

} // namespace detail
} // namespace zh

#undef TEMPLATE_CONTAINER_VIEW
#undef CONTAINER_VIEW