#pragma once

class id_manager {
private:
	size_t free_id;
public:
	size_t get();
	size_t last();
	void set_next(size_t next);
	void undo();
	void reset();

	constexpr id_manager();
};

inline size_t id_manager::get() {
	return free_id++;
}

inline size_t id_manager::last() {
	return free_id-1;
}

inline void id_manager::set_next(size_t next) {
	free_id = next;
}

inline void id_manager::undo() {
	set_next(last());
}

inline void id_manager::reset() {
	set_next(0);
}

inline constexpr id_manager::id_manager() :
	free_id (0) {
}
