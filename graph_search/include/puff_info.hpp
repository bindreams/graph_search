#pragma once

class puff_info {
private:
	std::size_t async_calls_ctor_;
	std::size_t async_calls_contains_;

public:
	std::size_t async_calls_ctor() {
		return async_calls_ctor_;
	}

	std::size_t async_calls_contains() {
		return async_calls_contains_;
	}

	template <class T>
	friend class puff;

	constexpr puff_info() :
		async_calls_ctor_(0),
		async_calls_contains_(0) {
	}
};