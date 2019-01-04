#pragma once
#include <type_traits>
#include <iterator>

namespace zh {

template<class Iterator>
struct is_input
	: std::is_base_of<
		std::input_iterator_tag,
		typename std::iterator_traits<Iterator>::iterator_category> {
};

template <class Iterator>
constexpr bool is_input_v = is_input<Iterator>::value;

} // namespace zh