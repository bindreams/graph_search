#pragma once
#include <iterator>
#include <memory>
#include <type_traits>
#include "deriveable_ptr.hpp"
#include "type_traits/is_function_like.hpp"
#include "fseq.hpp"
#include "as_functor.hpp"

// For convenience
#define TEMPLATE_PROXY_ITERATOR  template <class Iterator,  class Functor,  class... Functors>
#define TEMPLATE_PROXY_ITERATOR_ template <class Iterator_, class Functor_, class... Functors_>
#define PROXY_ITERATOR  proxy_iterator<Iterator,  Functor,  Functors...>
#define PROXY_ITERATOR_ proxy_iterator<Iterator_, Functor_, Functors_...>
// Undefined in proxy_iterator.inl

namespace zh {

TEMPLATE_PROXY_ITERATOR
class proxy_iterator : 
	// If Iterator is a pointer, derives from a wrapper class.
	// Otherwise, derives straight from Iterator.
	private make_deriveable<Iterator>,

	// Functors... contains additional functors to be applied in sequence.
	// if no additional functors were passed, derive from Functor.
	// Otherwise, derive from a functor sequence.
	private std::conditional_t<sizeof...(Functors) == 0,
		Functor, fseq<Functor, Functors...>> {
public:
	// Member types -----------------------------------------------------------

	// Iterator that was used to instantiate proxy_iterator
	using iterator_type = Iterator;
	using functor_type = std::conditional_t<
		sizeof...(Functors) == 0,
		Functor, 
		fseq<Functor, Functors...>
	>;

private:
	// Private types and functions --------------------------------------------

	// Actual base iterator type from which the class is derived
	using base = make_deriveable<Iterator>;

	base& iterator() noexcept;
	const base& iterator() const noexcept;

	functor_type& functor() noexcept;
	const functor_type& functor() const noexcept;

public:
	// Member types (cont.) ---------------------------------------------------
	using value_type
		= typename std::remove_reference_t<decltype(
			std::declval<functor_type>()(*std::declval<iterator_type>()))>;
	using difference_type
		= typename std::iterator_traits<iterator_type>::difference_type;
	using pointer
		= value_type * ;
	using reference
		= value_type & ;
	using iterator_category
		= typename std::iterator_traits<iterator_type>::iterator_category;

	// Constructors -----------------------------------------------------------
	explicit constexpr proxy_iterator(
		const iterator_type& other = iterator_type(),
		const functor_type& functor = functor_type())
		noexcept(
			std::is_nothrow_constructible_v<base, const iterator_type&> &&
			std::is_nothrow_copy_constructible_v<functor_type>);

	explicit constexpr proxy_iterator(const functor_type& functor)
		noexcept(noexcept(proxy_iterator(iterator_type(), functor)));

	// Convert from related proxy iterators
	// Example: proxy_iterator<iterator, ...> to 
	//          proxy_iterator<const_iterator, ...>
	template<class Iter, class = 
		std::enable_if_t<std::is_constructible_v<iterator_type, const Iter&>>>
	constexpr proxy_iterator(const proxy_iterator<Iter, functor_type>& other)
		noexcept(
			std::is_nothrow_constructible_v<base, const Iter&> &&
			std::is_nothrow_default_constructible_v<functor_type>);

	// Converters -------------------------------------------------------------

	// Unlike private methods that return references, these methods return
	// copies. Also, while private method iterator() returns base, get_iterator
	// returns iterator_type, which may or may not be different.
	constexpr iterator_type get_iterator() const noexcept;
	constexpr functor_type get_functor() const noexcept;

	// Member functions -------------------------------------------------------

	// The noexcept spec in the following two functions is equivalent to
	// noexcept(noexcept(std::addressof(functor()(*iterator()))))
	// The added boilerplate std::declval<PROXY_ITERATOR const>() is required
	// to circumvent MSVC bug that reports ambiguous call to overloaded
	// functions functor() and iterator().

	constexpr decltype(auto) operator->() const
		noexcept(noexcept(std::addressof(
			std::declval<PROXY_ITERATOR const>().functor()(*
			std::declval<PROXY_ITERATOR const>().iterator()))));

	constexpr decltype(auto) operator*() const
		noexcept(noexcept(
			std::declval<PROXY_ITERATOR const>().functor()(*
			std::declval<PROXY_ITERATOR const>().iterator())));

	// Some of the following methods may or may not be present in base iterator.
	// Since this is a templated class, these methods will fail to compile only
	// if used somewhere in the program. No SFINAE needed.

	constexpr decltype(auto) operator[](difference_type idx) const noexcept;

	constexpr proxy_iterator& operator++() noexcept;
	constexpr proxy_iterator operator++(int) noexcept;

	constexpr proxy_iterator& operator--() noexcept;
	constexpr proxy_iterator operator--(int) noexcept;

	constexpr proxy_iterator& operator+=(difference_type offset) noexcept;
	constexpr proxy_iterator& operator-=(difference_type offset) noexcept;

	// Friends ----------------------------------------------------------------
	TEMPLATE_PROXY_ITERATOR_ friend constexpr bool operator==(const PROXY_ITERATOR_& lhs, const PROXY_ITERATOR_& rhs);
	TEMPLATE_PROXY_ITERATOR_ friend constexpr bool operator< (const PROXY_ITERATOR_& lhs, const PROXY_ITERATOR_& rhs);
	
	TEMPLATE_PROXY_ITERATOR_
	friend constexpr PROXY_ITERATOR_ operator+(
			const PROXY_ITERATOR_& lhs, 
			typename PROXY_ITERATOR_::difference_type offset);
	
	TEMPLATE_PROXY_ITERATOR_
	friend constexpr PROXY_ITERATOR_ operator-(
			const PROXY_ITERATOR_& lhs, 
			typename PROXY_ITERATOR_::difference_type offset);

	TEMPLATE_PROXY_ITERATOR_
	friend constexpr typename PROXY_ITERATOR_::difference_type operator-(
			const PROXY_ITERATOR_& lhs, 
			const PROXY_ITERATOR_& rhs);
};

TEMPLATE_PROXY_ITERATOR constexpr bool operator==(const PROXY_ITERATOR& lhs, const PROXY_ITERATOR& rhs);
TEMPLATE_PROXY_ITERATOR constexpr bool operator!=(const PROXY_ITERATOR& lhs, const PROXY_ITERATOR& rhs);
TEMPLATE_PROXY_ITERATOR constexpr bool operator< (const PROXY_ITERATOR& lhs, const PROXY_ITERATOR& rhs);
TEMPLATE_PROXY_ITERATOR constexpr bool operator> (const PROXY_ITERATOR& lhs, const PROXY_ITERATOR& rhs);
TEMPLATE_PROXY_ITERATOR constexpr bool operator<=(const PROXY_ITERATOR& lhs, const PROXY_ITERATOR& rhs);
TEMPLATE_PROXY_ITERATOR constexpr bool operator>=(const PROXY_ITERATOR& lhs, const PROXY_ITERATOR& rhs);

TEMPLATE_PROXY_ITERATOR 
constexpr PROXY_ITERATOR operator+(
	const PROXY_ITERATOR& lhs, 
	typename PROXY_ITERATOR::difference_type offset);

TEMPLATE_PROXY_ITERATOR 
constexpr PROXY_ITERATOR operator+(
	typename PROXY_ITERATOR::difference_type offset, 
	const PROXY_ITERATOR& lhs);

TEMPLATE_PROXY_ITERATOR 
constexpr PROXY_ITERATOR operator-(
	const PROXY_ITERATOR& lhs, 
	typename PROXY_ITERATOR::difference_type offset);

TEMPLATE_PROXY_ITERATOR 
constexpr typename PROXY_ITERATOR::difference_type operator-(
	const PROXY_ITERATOR& lhs, 
	const PROXY_ITERATOR& rhs);


} // namespace zh

// Expose default functors
#include "iterator_functors.hpp"

#include "proxy_iterator.inl"