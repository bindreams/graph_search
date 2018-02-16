#pragma once
#include <cstdint>
#include <iostream>
#include <iomanip>

template <class T>
std::uintptr_t tr(const T* const val, size_t trimdigits = 2) {
	return reinterpret_cast<std::uintptr_t>(val) % static_cast<size_t>(pow(10, trimdigits));
}