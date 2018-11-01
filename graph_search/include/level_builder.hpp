#pragma once
#include "deps/ska/bytell_hash_map.hpp"
#include "deps/zh/extra_traits.hpp"
#include "node_group.hpp"
#include "sector.hpp"
#include "functors/sector_functors.hpp"
#include "node.hpp"

// Builds one level of a puff
template <class T, bool is_multithreaded = false>
class level_builder;

template <class T>
class level_builder<T, false> {
private:
	using sources_t = ska::bytell_hash_map<node_group<T>, std::vector<const sector<T>*>>;

public:
	using results_t = ska::bytell_hash_set<sector<T>, sector_nodes_hash<T>, sector_nodes_equal<T>>;
	
private:
	sources_t sources;
	results_t results;

	void add_result(const sector<T>& rslt);
public:
	// Build a level from a container
	template <class Container, class = std::enable_if_t<
		zh::is_range_v<Container> &&
		std::is_same_v<typename Container::value_type, sector<T>>>>
	bool build(const Container& last_level);

	const results_t& result() const noexcept;
};

#include "inline/level_builder.inl"