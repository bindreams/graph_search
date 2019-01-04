#pragma once
#include <cstdint>
#include <limits> 
#include <set>
#include <vector>

#include "sector.hpp"
#include "graph.hpp"

template <class T>
class puff {
public:
	// level_type has sectors of one size
	using level_type = std::vector<sector<T>>;
	// container holds all the levels
	using container_type = std::vector<level_type>;

private:
	container_type sectors;

public:
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

	// Search for another puff in this puff
	std::set<graph_match> search(const puff<T>& other) const;

	const level_type& operator[](std::size_t idx) const;
};

template <class T>
bool operator==(const puff<T>& lhs, const puff<T>& rhs);

template <class T>
bool operator!=(const puff<T>& lhs, const puff<T>& rhs);

template <class T>
std::ostream& operator<<(std::ostream& os, const puff<T>& obj);

#include "puff.inl"