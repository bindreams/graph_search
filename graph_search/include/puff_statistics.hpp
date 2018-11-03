#pragma once

class puff_statistics {
private:
	std::size_t m_async_calls_contains = 0;

public:
	void add_async_call_contains() {
		m_async_calls_contains++;
	}

	std::size_t async_calls_contains() const {
		return m_async_calls_contains;
	}
};