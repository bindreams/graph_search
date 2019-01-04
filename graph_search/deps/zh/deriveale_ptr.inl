#pragma once
#include "deriveable_ptr.hpp"

namespace zh {

template <class Ptr>
template <class P, class>
constexpr deriveable_ptr<Ptr>::deriveable_ptr(P&& p) noexcept :
	ptr(p) {
}

template <class Ptr>
template <class P, class>
constexpr deriveable_ptr<Ptr>& deriveable_ptr<Ptr>::operator=(P&& p) {
	ptr = p;
}

template <class Ptr>
template <class P, class>
constexpr deriveable_ptr<Ptr>::operator P() const noexcept {
	return ptr;
}

template<class Ptr>
inline constexpr deriveable_ptr<Ptr>::operator Ptr() const noexcept {
	return ptr;
}

template <class Ptr>
constexpr std::remove_pointer_t<Ptr>& 
deriveable_ptr<Ptr>::operator*() const noexcept {
	return *ptr;
}

template <class Ptr>
constexpr Ptr 
deriveable_ptr<Ptr>::operator->() const noexcept {
	return ptr;
}

template <class Ptr>
constexpr std::remove_pointer_t<Ptr>&
deriveable_ptr<Ptr>::operator[](std::ptrdiff_t idx) const noexcept {
	return ptr[idx];
}

template <class Ptr>
constexpr deriveable_ptr<Ptr>& 
deriveable_ptr<Ptr>::operator++() noexcept {
	++ptr;
	return *this;
}

template <class Ptr>
constexpr deriveable_ptr<Ptr> 
deriveable_ptr<Ptr>::operator++(int) noexcept {
	deriveable_ptr tmp(*this);
	operator++();
	return tmp;
}

template <class Ptr>
constexpr deriveable_ptr<Ptr>& 
deriveable_ptr<Ptr>::operator--() noexcept {
	--ptr;
	return *this;
}

template <class Ptr>
constexpr deriveable_ptr<Ptr> 
deriveable_ptr<Ptr>::operator--(int) noexcept {
	deriveable_ptr tmp(*this);
	operator--();
	return tmp;
}

template <class Ptr>
constexpr deriveable_ptr<Ptr>& 
deriveable_ptr<Ptr>::operator+=(std::ptrdiff_t offset) noexcept {
	ptr += offset;
	return *this;
}

template <class Ptr>
constexpr deriveable_ptr<Ptr>& 
deriveable_ptr<Ptr>::operator-=(std::ptrdiff_t offset) noexcept {
	ptr -= offset;
	return *this;
}

template <class Ptr>
std::ostream& operator<<(std::ostream& os, deriveable_ptr<Ptr> obj) {
	return os << obj.ptr;
}

template <class Ptr>
std::istream& operator>>(std::istream& is, deriveable_ptr<Ptr> obj) {
	return is << obj.ptr;
}

template <class Ptr> constexpr bool operator==(deriveable_ptr<Ptr> lhs, deriveable_ptr<Ptr> rhs) { return lhs.ptr == rhs.ptr; }
template <class Ptr> constexpr bool operator!=(deriveable_ptr<Ptr> lhs, deriveable_ptr<Ptr> rhs) { return !operator==(lhs, rhs); }
template <class Ptr> constexpr bool operator< (deriveable_ptr<Ptr> lhs, deriveable_ptr<Ptr> rhs) { return lhs.ptr < rhs.ptr; }
template <class Ptr> constexpr bool operator> (deriveable_ptr<Ptr> lhs, deriveable_ptr<Ptr> rhs) { return  operator< (rhs, lhs); }
template <class Ptr> constexpr bool operator<=(deriveable_ptr<Ptr> lhs, deriveable_ptr<Ptr> rhs) { return !operator> (lhs, rhs); }
template <class Ptr> constexpr bool operator>=(deriveable_ptr<Ptr> lhs, deriveable_ptr<Ptr> rhs) { return !operator< (lhs, rhs); }

template <class Ptr>
constexpr deriveable_ptr<Ptr> operator+(deriveable_ptr<Ptr> lhs, std::ptrdiff_t offset) {
	lhs += offset;
	return lhs;
}

template <class Ptr>
constexpr deriveable_ptr<Ptr> operator+(std::ptrdiff_t offset, deriveable_ptr<Ptr> lhs) {
	lhs += offset;
	return lhs;
}

template <class Ptr>
constexpr deriveable_ptr<Ptr> operator-(deriveable_ptr<Ptr> lhs, std::ptrdiff_t offset) {
	lhs -= offset;
	return lhs;
}

template <class Ptr>
constexpr std::ptrdiff_t operator-(deriveable_ptr<Ptr> lhs, deriveable_ptr<Ptr> rhs) {
	return lhs.ptr - rhs.ptr;
}

} // namespace zh