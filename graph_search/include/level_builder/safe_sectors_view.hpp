#pragma once
#include <mutex>
#include "sector.hpp"

template <class T>
struct safe_sectors_view {
	mutable std::mutex rwmutex;
	std::vector<const sector<T>*> sectors;

	constexpr safe_sectors_view() = default;

	safe_sectors_view(const safe_sectors_view& other);
	safe_sectors_view(safe_sectors_view&& other) = default;

	safe_sectors_view& operator=(const safe_sectors_view& other);
	safe_sectors_view& operator=(safe_sectors_view&& other) = default;
};

#include "inline/safe_sectors_view.inl"
