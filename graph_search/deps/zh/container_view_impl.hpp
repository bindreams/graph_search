#pragma once
#include <type_traits>
#include "type_traits/is_same.hpp"
#include "type_traits/is_bidirectional.hpp"
#include <iterator>

namespace zh {
namespace detail {

template <
	class Container,
	class Iterator,
	class ConstIterator,
	class ReverseIterator = std::reverse_iterator<Iterator>,
	class ConstReverseIterator = std::reverse_iterator<ConstIterator>,
	// If Iterator is not bidirectional or ReverseIterator and 
	// ConstReverseIterator are void, no reverse_iterators will be used, and no
	// rbegin/rend methods provided. This is implemented through the following
	// bool template parameter, which should always be deduced and is hidden in
	// container_view typedef.
	bool use_reverse = 
		is_bidirectional_v<Iterator> &&
		!is_same_v<ReverseIterator, ConstReverseIterator, void>,
	// Additional args to forward to constructors of iterators.
	// Example: auto begin() { return Iterator(c.begin(), args...); }
	class... Args>
class container_view_impl;

} // namespace detail
} // namespace zh

#include "container_view_impl_reverse.hpp"
#include "container_view_impl_no_reverse.hpp"