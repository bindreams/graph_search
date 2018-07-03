#pragma once
#include "sorted_vector.hpp"

template <
	class T,
	class Compare = std::less<T>,
	class Allocator = std::allocator<T>>
	using vecset = sorted_vector<T, true, Compare, Allocator>;

template <
	class T,
	class Compare = std::less<T>,
	class Allocator = std::allocator<T>>
	using multivecset = sorted_vector<T, false, Compare, Allocator>;