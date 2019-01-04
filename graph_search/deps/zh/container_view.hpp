#pragma once
#include <type_traits>
#include <iterator>
#include "container_view_impl.hpp"

namespace zh {

// container_view
// This class provides access to a class using some custom iterators.
// For example, container_view can be used with proxy_iterators to "view"
// a container through a proxy, such as view an array of pointers as an array
// of normal elements, complete with the ability to access and modify elements.

// Note: to forward Args..., these typedefs need to explicitly define the bool
// argument, that should normally always be deduced.

template <
	class Container,
	class Iterator,
	class ConstIterator,
	class ReverseIterator = std::reverse_iterator<Iterator>,
	class ConstReverseIterator = std::reverse_iterator<ConstIterator>,
	class... Args>
using container_view = 
	detail::container_view_impl<
		Container, 
		Iterator, 
		ConstIterator, 
		ReverseIterator, 
		ConstReverseIterator,
		is_bidirectional_v<Iterator> &&	!is_same_v<ReverseIterator, ConstReverseIterator, void>,
		Args...
	>;

template <
	class Container,
	class ConstIterator,
	class ConstReverseIterator = std::reverse_iterator<ConstIterator>,
	class... Args>
using const_container_view = 
	detail::container_view_impl<
		Container const,
		ConstIterator,
		ConstIterator,
		ConstReverseIterator,
		ConstReverseIterator,
		is_bidirectional_v<ConstIterator> && !is_same_v<ConstReverseIterator, void>,
		Args...
	>;

// Explicitly forbid reverse_iterators
template <
	class Container,
	class Iterator,
	class ConstIterator,
	class... Args>
using forward_container_view = 
	detail::container_view_impl<
		Container,
		Iterator,
		ConstIterator,
		void,
		void,
		false,
		Args...
	>;

template <
	class Container,
	class ConstIterator,
	class... Args>
using const_forward_container_view = 
	detail::container_view_impl<
		Container const,
		ConstIterator,
		ConstIterator,
		void,
		void,
		false,
		Args...
	>;

} // namespace zh