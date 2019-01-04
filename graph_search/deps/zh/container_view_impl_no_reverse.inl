#pragma once
#include "container_view_impl_no_reverse.hpp"
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
	false, \
	Args...>

namespace zh {
namespace detail {

TEMPLATE_CONTAINER_VIEW
constexpr typename CONTAINER_VIEW::container_type& 
CONTAINER_VIEW::base() noexcept {
	return c;
}

TEMPLATE_CONTAINER_VIEW
constexpr const typename CONTAINER_VIEW::container_type& 
CONTAINER_VIEW::base() const noexcept {
	return c;
}

TEMPLATE_CONTAINER_VIEW
constexpr typename CONTAINER_VIEW::iterator_args&
CONTAINER_VIEW::args() noexcept {
	return static_cast<iterator_args&>(*this);
}

TEMPLATE_CONTAINER_VIEW
constexpr const typename CONTAINER_VIEW::iterator_args&
CONTAINER_VIEW::args() const noexcept {
	return static_cast<const iterator_args&>(*this);
}

TEMPLATE_CONTAINER_VIEW
constexpr CONTAINER_VIEW::
container_view_impl(Container& container) :
	c(container) {
}

TEMPLATE_CONTAINER_VIEW
template<class ...IteratorArgs>
inline constexpr CONTAINER_VIEW::
container_view_impl(Container& container, IteratorArgs&&... args) :
	iterator_args(args...),
	c(container) {
}

TEMPLATE_CONTAINER_VIEW
constexpr Iterator CONTAINER_VIEW::begin() noexcept {
	// Make Iterator from container's begin and any additional arguments passed
	// in a tuple. If no additional args were specified during container_view
	// template instantiation, this call is equivalent to
	// return Iterator(std::begin(c))
	return make_from_first_and_tuple<Iterator>(
		std::begin(c), args());
}

TEMPLATE_CONTAINER_VIEW
constexpr ConstIterator CONTAINER_VIEW::begin() const noexcept {
	return make_from_first_and_tuple<ConstIterator>(
		std::cbegin(c), args());
}

TEMPLATE_CONTAINER_VIEW
constexpr ConstIterator CONTAINER_VIEW::cbegin() const noexcept {
	return make_from_first_and_tuple<ConstIterator>(
		std::cbegin(c), args());
}

TEMPLATE_CONTAINER_VIEW
constexpr Iterator CONTAINER_VIEW::end() noexcept {
	return make_from_first_and_tuple<Iterator>(
		std::end(c), args());
}

TEMPLATE_CONTAINER_VIEW
constexpr ConstIterator CONTAINER_VIEW::end() const noexcept {
	return make_from_first_and_tuple<ConstIterator>(
		std::cend(c), args());
}

TEMPLATE_CONTAINER_VIEW
constexpr ConstIterator CONTAINER_VIEW::cend() const noexcept {
	return make_from_first_and_tuple<ConstIterator>(
		std::cend(c), args());
}

TEMPLATE_CONTAINER_VIEW
template <class>
constexpr std::size_t CONTAINER_VIEW::size() const noexcept {
	return std::size(c);
}

TEMPLATE_CONTAINER_VIEW
template <class>
constexpr bool CONTAINER_VIEW::empty() const noexcept {
	return std::empty(c);
}

TEMPLATE_CONTAINER_VIEW
constexpr typename CONTAINER_VIEW::reference
CONTAINER_VIEW::front() {
	return *begin();
}

TEMPLATE_CONTAINER_VIEW
constexpr typename CONTAINER_VIEW::const_reference
CONTAINER_VIEW::front() const {
	return *cbegin();
}

TEMPLATE_CONTAINER_VIEW
template <class, class>
constexpr typename CONTAINER_VIEW::reference
CONTAINER_VIEW::back() {
	return *std::prev(end());
}

TEMPLATE_CONTAINER_VIEW
template <class, class>
constexpr typename CONTAINER_VIEW::const_reference
CONTAINER_VIEW::back() const {
	return *std::prev(cend());
}

TEMPLATE_CONTAINER_VIEW
template <class, class>
constexpr typename CONTAINER_VIEW::reference
CONTAINER_VIEW::operator[](std::size_t index) {
	return *(begin() + index);
}

TEMPLATE_CONTAINER_VIEW
template <class, class>
constexpr typename CONTAINER_VIEW::const_reference
CONTAINER_VIEW::operator[](std::size_t index) const {
	return *(cbegin() + index);
}

} // namespace detail
} // namespace zh

#undef TEMPLATE_CONTAINER_VIEW
#undef CONTAINER_VIEW