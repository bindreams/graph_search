#pragma once
#include <type_traits>
#include <iterator>

namespace zh {

template<class Iterator>
struct is_random_access
	: std::is_base_of<
		std::random_access_iterator_tag,
		typename std::iterator_traits<Iterator>::iterator_category> {
};

template <class Iterator>
constexpr bool is_random_access_v = is_random_access<Iterator>::value;

} // namespace zh