#pragma once

class id_manager {
	size_t free_id;
public:
	size_t get();

	constexpr id_manager();
};

inline size_t id_manager::get() {
	return free_id++;
}

inline constexpr id_manager::id_manager() :
	free_id (0) {
}
