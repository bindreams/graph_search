#pragma once
#include <future>
#include "level_builder.hpp"
#include "util/advance.hpp"
#include "util/enviroment.hpp"

template<class T>
inline void build_results<T>::add(const sector<T>& rslt) {
	auto[it, insert_happened] = this->insert(rslt);

	if (!insert_happened) {
		// If insertion did not take place, just join children
		it->join_children(rslt);
	}
}

template<class T>
inline void build_results<T>::join(const build_results& other) {
	for (const auto& sec : other) {
		add(sec);
	}
}

template<class T>
template<class Container, class>
inline bool level_builder<T, false>::build(const Container& last_level) {
	return build(std::begin(last_level), std::end(last_level));
}

template<class T>
template<class InputIt>
inline bool level_builder<T, false>::build(InputIt first, InputIt last) {
	GS_ASSERT(first->nodes.size() >= 2);
	sources.clear();
	results.clear();

	// For each sector in last level
	for (InputIt it = first; it != last; ++it) {
		const sector<T>& sector = *it;
		// Go over each possible part of its group
		// (see node_group::except_1)
		for (auto& part : sector.nodes.except_1()) {
			// Example: sector is abc, part is ac
			// In sources[part], go over all values, build a new sector with
			// each of them, and insert each one into results
			for (auto& source : sources[part]) {
				results.add({sector, *source});
			}
			// Finally, add this sector to sources
			sources[part].push_back(&sector);
		}
	}

	if (results.empty()) return false;
	return true;
}

template<class T>
inline const build_results<T>& level_builder<T, false>::result() const noexcept {
	return results;
}

template<class T>
const unsigned int level_builder<T, true>::worker_count = std::thread::hardware_concurrency() - 1;

template<class T>
template<class InputIt>
inline void level_builder<T, true>::populate(InputIt first, InputIt last) {
	// For each sector in last level
	for (InputIt it = first; it != last; ++it) {
		const sector<T>& sector = *it;
		// Go over each possible part of its group
		// (see node_group::except_1)
		for (const auto& part : sector.nodes.except_1()) {
			// Populate map with keys
			sources[part];
		}
	}
}

template<class T>
template<class InputIt>
inline build_results<T> level_builder<T, true>::build_safe(InputIt first, InputIt last) {
	//auto dur = ch::steady_clock::now() - times[std::this_thread::get_id()];
	//std::stringstream msg;
	//msg << "Thread " << std::this_thread::get_id() << " waited " << dur.count() << "ns" << std::endl;
	//std::cout << msg.str();
	build_results<T> rslt;

	// For each sector in last level
	for (InputIt it = first; it != last; ++it) {
		const sector<T>& sector = *it;
		// Go over each possible part of its group
		// (see node_group::except_1)
		for (const auto& part : sector.nodes.except_1()) {
			// Example: sector is abc, part is ac
			// In sources[part], go over all values, build a new sector with
			// each of them, and insert each one into results.
			//auto s = ch::steady_clock::now();

			// Call to sources[part] must not create a new element.
			// Otherwise container will be modified, which is not safe.
			GS_ASSERT(sources.count(part) != 0);
			std::unique_lock lk(sources[part].rwmutex);
			//auto dur = ch::steady_clock::now() - s;
			//std::stringstream msg;
			//msg << "Thread " << std::this_thread::get_id() << " waited " << dur.count() << "ns" << std::endl;
			//std::cout << msg.str();
			for (auto& source : sources[part].sectors) {
				rslt.add({sector, *source});
			}
			// Finally, add this sector to sources
			sources[part].sectors.push_back(&sector);
		}
	}

	//times[std::this_thread::get_id()] = ch::steady_clock::now();
	return rslt;
}

template<class T>
template<class Container, class>
inline bool level_builder<T, true>::build(const Container& last_level, std::size_t block_size) {
	return build(std::begin(last_level), std::end(last_level), block_size);
}

template<class T>
template<class InputIt>
inline bool level_builder<T, true>::build(InputIt first, InputIt last, std::size_t block_size) {
	GS_ASSERT(first->nodes.size() >= 2);
	sources.clear();
	results.clear();

	// Populate the sources
	populate(first, last);

	std::vector<std::future<build_results<T>>> async_results;
	async_results.reserve(worker_count);

	// Concurrently compute parts of solution in increments of block_size
	auto block_first = first;
	auto block_last = first;

	while (block_last != last) {
		// Move block_first to previous block's block_last
		block_first = block_last;
		// Advance block_last until last is reached
		zh::advance(block_last, last, block_size);

		// Asunc call
		async_results.push_back(pool.push([this, block_first, block_last](int) {
			return this->build_safe(block_first, block_last);
		}));
		//	&level_builder<T, true>::template build_safe<InputIt>, this,
		//	block_first, block_last));
	}

	// Unpack results and join them
	for (auto& future_rslt : async_results) {
		results.join(future_rslt.get());
	}
	//std::cout << "block_size: " << block_size << "; size: " << async_results.size() << std::endl;
	//std::cin.get();

	if (results.empty()) return false;
	return true;
}

template<class T>
inline level_builder<T, true>::level_builder() :
	pool(worker_count) {
}

template<class T>
inline const build_results<T>& level_builder<T, true>::result() const noexcept {
	return results;
}