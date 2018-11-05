#pragma once
#include <algorithm>
#include <limits>
#include <set>
#include <vector>
#include <list>
#include <deque>
#include <thread>
#include <cmath>
#include <boost/container/flat_set.hpp>

#include "sector.hpp"
#include "graph.hpp"
#include "puff_statistics.hpp"
#include "level_builder.hpp"
#include "util/enviroment.hpp"
using boost::container::flat_set;

template <class T>
class puff {
public:
	// level_container has sectors of one size
	using level_container = std::vector<sector<T>>;
	// container holds all the levels
	using container = std::vector<level_container>;

private:
	container sectors;

public:
#ifdef GS_COLLECT_STATS
	mutable puff_statistics stats;
#endif
	// Constructors ===========================================================
	puff() = default;
	puff(const graph<T>& gr, std::size_t max_depth = std::numeric_limits<std::size_t>::max());

	puff(const puff& other) = default;
	puff(puff&& other) = default;

	puff& operator=(const puff& other) = default;
	puff& operator=(puff&& other) = default;

	// Observers ==============================================================

	// How many levels this puff has
	std::size_t depth() const noexcept;

	std::size_t count_edges() const noexcept;
	std::size_t count_sectors() const noexcept;
	std::size_t size_in_bytes() const noexcept;

	// Checks if this puff contains other puff
	std::set<graph_match> contains(const puff<T>& other) const;

	const level_container& operator[](std::size_t idx) const noexcept;
};

template <class T>
std::ostream& operator<<(std::ostream& os, const puff<T>& obj);

#include "inline/puff.inl"