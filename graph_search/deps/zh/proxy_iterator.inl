#pragma once
#include "proxy_iterator.hpp"

namespace zh {

TEMPLATE_PROXY_ITERATOR
typename PROXY_ITERATOR::base&
PROXY_ITERATOR::iterator() noexcept {
	return static_cast<base&>(*this);
}

TEMPLATE_PROXY_ITERATOR
const typename PROXY_ITERATOR::base&
PROXY_ITERATOR::iterator() const noexcept {
	return static_cast<const base&>(*this);
}

TEMPLATE_PROXY_ITERATOR
typename PROXY_ITERATOR::functor_type&
PROXY_ITERATOR::functor() noexcept {
	return static_cast<functor_type&>(*this);
}

TEMPLATE_PROXY_ITERATOR
const typename PROXY_ITERATOR::functor_type&
PROXY_ITERATOR::functor() const noexcept {
	return static_cast<const functor_type&>(*this);
}

TEMPLATE_PROXY_ITERATOR
constexpr PROXY_ITERATOR::
proxy_iterator(const iterator_type& other, const functor_type& functor)
	noexcept(
		std::is_nothrow_constructible_v<base, const iterator_type&> &&
		std::is_nothrow_copy_constructible_v<functor_type>) :
	base(other),
	functor_type(functor) {
}

TEMPLATE_PROXY_ITERATOR
constexpr PROXY_ITERATOR::
proxy_iterator(const functor_type& functor)
noexcept(noexcept(proxy_iterator(iterator_type(), functor))) :
	proxy_iterator(iterator_type(), functor) {
}

TEMPLATE_PROXY_ITERATOR
template <class Iter, class>
constexpr PROXY_ITERATOR::
proxy_iterator(const proxy_iterator<Iter, functor_type>& other)
noexcept(
	std::is_nothrow_constructible_v<base, const Iter&> &&
	std::is_nothrow_default_constructible_v<functor_type>) :
	base(other.iterator()) {
}

TEMPLATE_PROXY_ITERATOR
constexpr typename PROXY_ITERATOR::iterator_type
PROXY_ITERATOR::get_iterator() const noexcept {
	return iterator();
}

TEMPLATE_PROXY_ITERATOR
constexpr typename PROXY_ITERATOR::functor_type
PROXY_ITERATOR::get_functor() const noexcept {
	return functor();
}

TEMPLATE_PROXY_ITERATOR
constexpr decltype(auto) PROXY_ITERATOR::operator->() const
	noexcept(noexcept(std::addressof(
		std::declval<PROXY_ITERATOR const>().functor()(*
		std::declval<PROXY_ITERATOR const>().iterator())))) {
	return std::addressof(functor()(*iterator()));
}

TEMPLATE_PROXY_ITERATOR
constexpr decltype(auto) PROXY_ITERATOR::operator*() const
	noexcept(noexcept(
		std::declval<PROXY_ITERATOR const>().functor()(*
		std::declval<PROXY_ITERATOR const>().iterator()))) {
	return functor()(*iterator());
}

TEMPLATE_PROXY_ITERATOR
constexpr decltype(auto)
PROXY_ITERATOR::operator[](difference_type idx) const noexcept {
	return iterator()[idx];
}

TEMPLATE_PROXY_ITERATOR
constexpr PROXY_ITERATOR&
PROXY_ITERATOR::operator++() noexcept {
	++iterator();
	return *this;
}

TEMPLATE_PROXY_ITERATOR
inline constexpr PROXY_ITERATOR
PROXY_ITERATOR::operator++(int) noexcept {
	proxy_iterator tmp(*this);
	operator++();
	return tmp;
}

TEMPLATE_PROXY_ITERATOR
constexpr PROXY_ITERATOR&
PROXY_ITERATOR::operator--() noexcept {
	--iterator();
	return *this;
}

TEMPLATE_PROXY_ITERATOR
constexpr PROXY_ITERATOR
PROXY_ITERATOR::operator--(int) noexcept {
	proxy_iterator tmp(*this);
	operator--();
	return tmp;
}

TEMPLATE_PROXY_ITERATOR
constexpr PROXY_ITERATOR&
PROXY_ITERATOR::operator+=(difference_type offset) noexcept {
	iterator() += offset;
	return *this;
}

TEMPLATE_PROXY_ITERATOR
constexpr PROXY_ITERATOR&
PROXY_ITERATOR::operator-=(difference_type offset) noexcept {
	iterator() -= offset;
	return *this;
}

TEMPLATE_PROXY_ITERATOR
constexpr bool operator==(const PROXY_ITERATOR& lhs, const PROXY_ITERATOR& rhs) {
	return lhs.iterator() == rhs.iterator();
}

TEMPLATE_PROXY_ITERATOR
constexpr bool operator!=(const PROXY_ITERATOR& lhs, const PROXY_ITERATOR& rhs) {
	return !(lhs == rhs);
}

TEMPLATE_PROXY_ITERATOR
constexpr bool operator< (const PROXY_ITERATOR& lhs, const PROXY_ITERATOR& rhs) {
	return lhs.iterator() < rhs.iterator();
}

TEMPLATE_PROXY_ITERATOR
constexpr bool operator> (const PROXY_ITERATOR& lhs, const PROXY_ITERATOR& rhs) {
	return rhs < lhs;
}

TEMPLATE_PROXY_ITERATOR
constexpr bool operator<=(const PROXY_ITERATOR& lhs, const PROXY_ITERATOR& rhs) {
	return !(lhs > rhs);
}

TEMPLATE_PROXY_ITERATOR
constexpr bool operator>=(const PROXY_ITERATOR& lhs, const PROXY_ITERATOR& rhs) {
	return !(lhs < rhs);
}

TEMPLATE_PROXY_ITERATOR
constexpr PROXY_ITERATOR
operator+(const PROXY_ITERATOR& lhs, typename PROXY_ITERATOR::difference_type offset) {
	return PROXY_ITERATOR(lhs.iterator() + offset, lhs.functor());
}

TEMPLATE_PROXY_ITERATOR
constexpr PROXY_ITERATOR
operator+(typename PROXY_ITERATOR::difference_type offset, const PROXY_ITERATOR& lhs) {
	return lhs + offset;
}

TEMPLATE_PROXY_ITERATOR
constexpr PROXY_ITERATOR
operator-(const PROXY_ITERATOR& lhs, typename PROXY_ITERATOR::difference_type offset) {
	return PROXY_ITERATOR(lhs.iterator() - offset, lhs.functor());
}

TEMPLATE_PROXY_ITERATOR
constexpr typename PROXY_ITERATOR::difference_type
operator-(const PROXY_ITERATOR& lhs, const PROXY_ITERATOR& rhs) {
	return lhs.iterator() - rhs.iterator();
}

} // namespace zh

#undef TEMPLATE_PROXY_ITERATOR
#undef TEMPLATE_PROXY_ITERATOR_
#undef PROXY_ITERATOR
#undef PROXY_ITERATOR_
// Defined in proxy_iterator.hpp