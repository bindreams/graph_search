#pragma once
#include <list>
#include "deps/ska/bytell_hash_map.hpp"
#include "deps/zh/extra_traits.hpp"
#include "node_group.hpp"
#include "sector.hpp"
#include "node.hpp"

template<class T>
struct nodes_hash {
	std::size_t operator()(const sector<T>& obj) const {
		return std::hash<node_group<T>>()(obj.nodes);
	}
};

template<class T>
struct nodes_equal {
	std::size_t operator()(const sector<T>& lhs, const sector<T>& rhs) const {
		return lhs.nodes == rhs.nodes;
	}
};

// Builds one level of a puff
template <class T>
class level_builder {
private:
	using sources_t = ska::bytell_hash_map<node_group<T>, std::vector<const sector<T>*>>;

public:
	using results_t = ska::bytell_hash_set<sector<T>, nodes_hash<T>, nodes_equal<T>>;
	
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

template<class T>
inline void level_builder<T>::add_result(const sector<T>& rslt) {
	auto[it, insert_happened] = results.insert(rslt);

	if (!insert_happened) {
		// If insertion did not take place, just join children
		it->join_children(rslt);
	}
}

template<class T>
template<class Container, class>
inline bool level_builder<T>::build(const Container& last_level) {
	if (last_level.begin()->nodes.size() < 2) 
		throw std::runtime_error("level_builder: builder needs last_level to have sectors of size 2 or more");
	sources.clear();
	results.clear();

	// For each sector in last level
	for (auto& sector : last_level) {
		// Go over each possible part of its group
		// (see node_group::except_1)
		for (auto& part : sector.nodes.except_1()) {
			// Example: sector is abc, part is ac
			// In sources[part], go over all values, build a new sector with
			// each of them, and insert each one into results
			for (auto& source : sources[part]) {
				add_result({sector, *source});
			}
			// Finally, add this sector to sources
			sources[part].push_back(&sector);
		}
	}

	if (results.empty()) return false;
	return true;
}

template<class T>
inline const typename level_builder<T>::results_t& 
level_builder<T>::result() const noexcept {
	return results;
}
