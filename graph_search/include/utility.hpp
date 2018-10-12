#pragma once
#include <vector>

template <class T>
inline std::vector<T> range(T start, T end, T step = 1) {
	std::vector<T> rslt;

	for (T i = start; i < end; i += step) {
		rslt.push_back(i);
	}

	return rslt;
}