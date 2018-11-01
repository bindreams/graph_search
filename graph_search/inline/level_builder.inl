#pragma once
#include "level_builder.hpp"

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
