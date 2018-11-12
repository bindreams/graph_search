#pragma once
#include "deps/ska/bytell_hash_map.hpp"
#include "sector.hpp"

template <class T>
class build_result : public ska::bytell_hash_set<sector<T>, sector_nodes_hash<T>, sector_nodes_equal<T>> {
public:
	void add(const sector<T>& rslt);
	void join(const build_result& other);
};

#include "inline/build_result.inl"