#pragma once
#include <type_traits>
#include <iterator>

namespace zh {

template<class Iterator>
struct is_forward
	: std::is_base_of<
		std::forward_iterator_tag,
		typename std::iterator_traits<Iterator>::iterator_category> {
};

template <class Iterator>
constexpr bool is_forward_v = is_forward<Iterator>::value;

} // namespace zh