#pragma once
#include <type_traits>
#include <cstdint>
#include <iostream>
#include <iterator>

namespace zh {

template <class Ptr>
class deriveable_ptr {
	static_assert(std::is_pointer_v<Ptr>,
		"deriveable_ptr: template argument must be a pointer");
private:
	Ptr ptr;

public:
	// Constructors -----------------------------------------------------------
	constexpr deriveable_ptr() noexcept = default;

	template <class P, class =
		std::enable_if_t<std::is_constructible_v<Ptr, P>>>
	constexpr deriveable_ptr(P&& p) noexcept;

	constexpr deriveable_ptr(const deriveable_ptr&) noexcept = default;
	constexpr deriveable_ptr(deriveable_ptr&&) noexcept = default;

	constexpr deriveable_ptr& operator=(const deriveable_ptr&) noexcept = default;
	constexpr deriveable_ptr& operator=(deriveable_ptr&&) noexcept = default;

	template <class P, class =
		std::enable_if_t<std::is_convertible_v<P, Ptr>>>
	constexpr deriveable_ptr& operator=(P&& p);

	template <class P, class =
		std::enable_if_t<std::is_convertible_v<Ptr, P>>>
	constexpr operator P() const noexcept;

	constexpr operator Ptr() const noexcept;

	// Member functions -------------------------------------------------------
	constexpr std::remove_pointer_t<Ptr>& operator*() const noexcept;
	constexpr Ptr operator->() const noexcept;

	constexpr std::remove_pointer_t<Ptr>& operator[](std::ptrdiff_t idx) const noexcept;

	constexpr deriveable_ptr& operator++() noexcept;
	constexpr deriveable_ptr operator++(int) noexcept;

	constexpr deriveable_ptr& operator--() noexcept;
	constexpr deriveable_ptr operator--(int) noexcept;

	constexpr deriveable_ptr& operator+=(std::ptrdiff_t offset) noexcept;
	constexpr deriveable_ptr& operator-=(std::ptrdiff_t offset) noexcept;

	template <class Ptr_> friend std::ostream& operator<<(std::ostream& os, deriveable_ptr<Ptr_> obj);
	template <class Ptr_> friend std::istream& operator>>(std::istream& is, deriveable_ptr<Ptr_> obj);

	template <class Ptr_> friend constexpr bool operator==(deriveable_ptr<Ptr_> lhs, deriveable_ptr<Ptr_> rhs);
	template <class Ptr_> friend constexpr bool operator< (deriveable_ptr<Ptr_> lhs, deriveable_ptr<Ptr_> rhs);

	template <class Ptr_> friend constexpr std::ptrdiff_t operator-(deriveable_ptr<Ptr_> lhs, deriveable_ptr<Ptr_> rhs);
};

template <class Ptr, class = std::enable_if_t<std::is_pointer_v<Ptr>>>
deriveable_ptr(Ptr)->deriveable_ptr<Ptr>;

template <class Iterator>
using make_deriveable = std::conditional_t<
	std::is_pointer_v<Iterator>,
	deriveable_ptr<Iterator>,
	Iterator
>;

template <class Ptr> std::ostream& operator<<(std::ostream& os, deriveable_ptr<Ptr> obj);
template <class Ptr> std::istream& operator>>(std::istream& is, deriveable_ptr<Ptr> obj);

template <class Ptr> constexpr bool operator==(deriveable_ptr<Ptr> lhs, deriveable_ptr<Ptr> rhs);
template <class Ptr> constexpr bool operator!=(deriveable_ptr<Ptr> lhs, deriveable_ptr<Ptr> rhs);
template <class Ptr> constexpr bool operator< (deriveable_ptr<Ptr> lhs, deriveable_ptr<Ptr> rhs);
template <class Ptr> constexpr bool operator> (deriveable_ptr<Ptr> lhs, deriveable_ptr<Ptr> rhs);
template <class Ptr> constexpr bool operator<=(deriveable_ptr<Ptr> lhs, deriveable_ptr<Ptr> rhs);
template <class Ptr> constexpr bool operator>=(deriveable_ptr<Ptr> lhs, deriveable_ptr<Ptr> rhs);

template <class Ptr> constexpr deriveable_ptr<Ptr> operator+(deriveable_ptr<Ptr> lhs, std::ptrdiff_t offset);
template <class Ptr> constexpr deriveable_ptr<Ptr> operator+(std::ptrdiff_t offset,   deriveable_ptr<Ptr> lhs);
template <class Ptr> constexpr deriveable_ptr<Ptr> operator-(deriveable_ptr<Ptr> lhs, std::ptrdiff_t offset);
template <class Ptr> constexpr std::ptrdiff_t      operator-(deriveable_ptr<Ptr> lhs, deriveable_ptr<Ptr> rhs);

} // namespace zh

namespace std {

template <class Ptr>
struct iterator_traits<zh::deriveable_ptr<Ptr>>
	: iterator_traits<Ptr> {
};

}

#include "deriveale_ptr.inl"