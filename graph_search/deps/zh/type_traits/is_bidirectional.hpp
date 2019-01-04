#pragma once
#include <type_traits>
#include <iterator>

namespace zh {

template<class Iterator>
struct is_bidirectional
	: std::is_base_of<
		std::bidirectional_iterator_tag,
		typename std::iterator_traits<Iterator>::iterator_category> {
};

template <class Iterator>
constexpr bool is_bidirectional_v = is_bidirectional<Iterator>::value;

} // namespace zh