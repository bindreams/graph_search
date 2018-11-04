#pragma once
#include <shared_mutex>
#include "deps/ska/bytell_hash_map.hpp"
#include "deps/zh/extra_traits.hpp"
#include "deps/ctpl_stl.h"
#include "node_group.hpp"
#include "sector.hpp"
#include "functors/sector_functors.hpp"
#include "node.hpp"

template <class T>
class build_results : public ska::bytell_hash_set<sector<T>, sector_nodes_hash<T>, sector_nodes_equal<T>> {
public:
	void add(const sector<T>& rslt);
	void join(const build_results& other);
};

// Builds one level of a puff
template <class T, bool is_multithreaded = true>
class level_builder;

template <class T>
class level_builder<T, false> {
private:
	using sources_t = ska::bytell_hash_map<node_group<T>, std::vector<const sector<T>*>>;
	
	sources_t sources;
	build_results<T> results;

public:
	// Build a level from a container
	template <class Container, class = std::enable_if_t<
		zh::is_range_v<Container> &&
		std::is_same_v<typename Container::value_type, sector<T>>>>
	bool build(const Container& last_level);

	template <class InputIt>
	bool build(InputIt first, InputIt last);

	const build_results<T>& result() const noexcept;
};

template <class T>
struct safe_sectors_view {
	mutable std::mutex rwmutex;
	std::vector<const sector<T>*> sectors;

	constexpr safe_sectors_view() = default;

	safe_sectors_view(const safe_sectors_view& other) {
		std::unique_lock lk(other.rwmutex);
		sectors = other.sectors;
	}

	safe_sectors_view& operator=(const safe_sectors_view& other) {
		std::unique_lock lk(other.rwmutex);
		sectors = other.sectors;
		return *this;
	}
};

template <class T>
class level_builder<T, true> {
private:
	using sources_t = ska::bytell_hash_map<node_group<T>, safe_sectors_view<T>>;

	sources_t sources;
	build_results<T> results;
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
	build_results<T> build_safe(InputIt first, InputIt last);
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

	const build_results<T>& result() const noexcept;
};

#include "inline/level_builder.inl"