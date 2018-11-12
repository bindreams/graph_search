#pragma once
#include "deps/ska/bytell_hash_map.hpp"
#include "deps/zh/extra_traits.hpp"
#include "deps/ctpl_stl.h"
#include "level_builder.hpp"
#include "node_group.hpp"
#include "safe_sectors_view.hpp"
#include "build_result.hpp"

template <class T>
class level_builder<T, true> {
private:
	using sources_t = ska::bytell_hash_map<node_group<T>, safe_sectors_view<T>>;

	sources_t sources;
	build_result<T> results;
	ctpl::thread_pool pool;

	static const unsigned int worker_count;

	// Populate sources with default values so that no call to operator[]
	// invalidates iterators or modifies the container.
	// Required before calling build_safe
	template <class InputIt>
	void populate(InputIt first, InputIt last);

	// Build a solution (thread-safe)
	// results member is not used. Instead, a variable is returned from this function
	// [warn] Call populate on all elements before calling this
	template <class InputIt>
	build_result<T> build_safe(InputIt first, InputIt last);
public:
	// Constructors ===========================================================
	level_builder();

	level_builder(const level_builder& other) = delete;
	level_builder(level_builder&& other) = delete;

	// Build a level from a container
	template <class Container, class = std::enable_if_t<
		zh::is_range_v<Container> &&
		std::is_same_v<typename Container::value_type, sector<T>>>>
		bool build(const Container& last_level);

	// Build a level from two iterators.
	// Optional parameter accepts container size, if using
	// std::distance is undesireable
	template <class InputIt>
	bool build(InputIt first, InputIt last, std::size_t size);
	template <class InputIt>
	bool build(InputIt first, InputIt last);

	build_result<T>& result() noexcept;
	const build_result<T>& result() const noexcept;
};

#include "inline/level_builder_mthread.inl"