#pragma once
#include <cassert>
#include <random>
#include <type_traits>
#include <iterator>

#include "deps/zh/extra_traits.hpp"
#include "generator.hpp"

namespace zh {

// advance ====================================================================
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

// select =====================================================================
// Select a random iterator in range [first, last), optional generator.
// Behavior is undefined if first is equal to last.
template <class InputIt>
InputIt select(InputIt first, InputIt last) {
	assert(first != last);
	std::advance(first, generator<std::size_t>()(0, std::distance(first, last)-1));
	return first;
}

template <class InputIt, class Gen>
InputIt select(InputIt first, InputIt last, Gen&& gen) {
	assert(first != last);

	std::uniform_int_distribution<std::size_t> dist(0, std::distance(first, last) - 1);
	std::advance(first, dist(gen()));

	return first;
}

// Select a random iterator in container, optional generator.
// Behavior is undefined if container is empty.
template <class Container, class = std::enable_if_t<
	zh::is_range_v<Container> &&
	zh::has_size_v<Container>>>
auto select(Container&& cont) {
	// Auto return type to select const_iterator where needed
	assert(std::size(std::forward<Container>(cont)) > 0);

	return std::next(std::begin(std::forward<Container>(cont)),
		generator<std::size_t>()(0, std::size(std::forward<Container>(cont)) - 1));
}

template <class Container, class Gen, class = std::enable_if_t<
	zh::is_range_v<Container> &&
	zh::has_size_v<Container>>>
auto select(Container&& cont, Gen&& gen) {
	// Auto return type to select const_iterator where needed
	assert(std::size(std::forward<Container>(cont)) > 0);

	std::uniform_int_distribution<std::size_t> dist(0, std::size(std::forward<Container>(cont)) - 1);
	return std::next(std::begin(std::forward<Container>(cont)), dist(gen()));
}

}