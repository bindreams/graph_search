#pragma once
#include <iterator>
#include <type_traits>

namespace zh {

template <class BaseIterator, class FunctorType>
class proxy_iterator : public BaseIterator, private FunctorType {
public:
	// Member types ============================================================
	using value_type
		= typename std::remove_reference<decltype(FunctorType()(*std::declval<BaseIterator>()))>::type;
	using difference_type
		= typename std::iterator_traits<BaseIterator>::difference_type;
	using pointer
		= value_type * ;
	using reference
		= value_type & ;
	using iterator_category
		= typename std::iterator_traits<BaseIterator>::iterator_category;

public:
	using BaseIterator::BaseIterator;

	explicit proxy_iterator(const BaseIterator& other) :
		BaseIterator(other) {
	}

	// Convert from related proxy iterators
	// Example: proxy_iterator<iterator, ...> to proxy_iterator<const_iterator, ...>
	template<class Iter, class = typename std::enable_if<std::is_convertible<Iter, BaseIterator>::value>::type>
	proxy_iterator(const proxy_iterator<Iter, FunctorType>& other) :
		BaseIterator(static_cast<const Iter&>(other)) {
	}

	decltype(auto) operator->() const {
		return &FunctorType::operator()(*static_cast<const BaseIterator&>(*this));
	}

	decltype(auto) operator*() const {
		return FunctorType::operator()(*static_cast<const BaseIterator&>(*this));
	}
};

} // namespace zh

// Expose default functors
#include "iterator_functors.hpp"