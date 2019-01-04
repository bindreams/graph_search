#pragma once
#include "level_builder.hpp"

#include "deps/zh/type_traits.hpp"
#include "build_result.hpp"

template <class T>
class level_builder<T, false> {
private:
	using sources_t = ska::bytell_hash_map<node_group<T>, std::vector<const sector<T>*>>;

	sources_t sources;
	build_result<T> results;

public:
	// Build a level from a container
	template <class Container, class = std::enable_if_t<
		zh::is_range_v<Container> &&
		std::is_same_v<typename Container::value_type, sector<T>>>>
		bool build(const Container& last_level);

	template <class InputIt>
	bool build(InputIt first, InputIt last);

	build_result<T>& result() noexcept;
	const build_result<T>& result() const noexcept;
};

#include "level_builder_1thread.inl"