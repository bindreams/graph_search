#pragma once
#include <type_traits>
#include <iterator>

namespace zh {

template<class Iterator>
struct is_output
	: std::is_base_of<
		std::output_iterator_tag,
		typename std::iterator_traits<Iterator>::iterator_category> {
};

template <class Iterator>
constexpr bool is_output_v = is_output<Iterator>::value;

} // namespace zh