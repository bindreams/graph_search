#pragma once
#include <type_traits>
#include <iterator>

namespace zh {

template <class InputIt, class Distance>
constexpr void advance(InputIt& it, Distance n) {
	std::advance(it, n);
}


// Advance iterator for distance n or until limit
template <class InputIt, class Distance>
constexpr bool advance(InputIt& it, InputIt limit, Distance n) {
	// Case: random_access_iterator
	if constexpr (std::is_same_v<
		std::random_access_iterator_tag,
		typename std::iterator_traits<InputIt>::iterator_category>) {
		
		if ((limit - it) <= n) {
			it = limit;
			return true;
		}
		std::advance(it, n);
		return false;
	}
	
	// Case: positive offset
	if (n >= 0) {
		for (Distance i = 0; i < n; ++i) {
			++it;
			if (it == limit) return true;
		}

		return false;
	}

	// Case: negative offset
	for (Distance i = 0; i > n; --i) {
		--it;
		if (it == limit) return true;
	}
	
	return false;
}

}