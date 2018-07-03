#pragma once
#include <map>

template <class T>
class node;

template <class T, bool is_const, bool is_reversed>
class graph_iterator {
	template <class T>
	friend class graph_impl;
private:
	using base_iterator = std::conditional_t<is_const,
		std::conditional_t<is_reversed,
		typename std::map<size_t, node<T>>::const_reverse_iterator,
		typename std::map<size_t, node<T>>::const_iterator>,
		std::conditional_t<is_reversed,
		typename std::map<size_t, node<T>>::reverse_iterator,
		typename std::map<size_t, node<T>>::iterator>>;

	base_iterator iter;

	graph_iterator(const base_iterator& iter_);

public:
	// Member types ============================================================
	using value_type = node<T>;
	using difference_type = 
		typename base_iterator::difference_type;
	using pointer = node<T>*;
	using const_pointer = const node<T>*;
	using reference = node<T>&;
	using const_reference = const node<T>&;
	using iterator_category = 
		typename base_iterator::iterator_category;

	// Constuctors =============================================================
	graph_iterator() = default;
	graph_iterator(const graph_iterator& other) = default;
	graph_iterator(graph_iterator&& other) = default;

	graph_iterator& operator=(const graph_iterator& other) = default;
	graph_iterator& operator=(graph_iterator&& other) = default;

	// Member functions ========================================================
	template <class = std::enable_if_t<!is_const>>
	reference operator*();
	const_reference operator*() const;
	template <class = std::enable_if_t<!is_const>>
	pointer operator->();
	const_pointer operator->() const;

	graph_iterator& operator++();
	graph_iterator operator++(int);
	graph_iterator& operator--();
	graph_iterator operator--(int);

	template <class T_, bool is_const_, bool is_reversed_>
	friend bool operator==(
		const graph_iterator<T_, is_const_, is_reversed_>& lhs, 
		const graph_iterator<T_, is_const_, is_reversed_>& rhs);
	template <class T_, bool is_const_, bool is_reversed_>
	friend bool operator!=(
		const graph_iterator<T_, is_const_, is_reversed_>& lhs, 
		const graph_iterator<T_, is_const_, is_reversed_>& rhs);
};

template<class T, bool is_const, bool is_reversed>
inline graph_iterator<T, is_const, is_reversed>::
graph_iterator(const typename graph_iterator<T, is_const, is_reversed>::base_iterator& iter_) :
	iter(iter_) {
}

template<class T, bool is_const, bool is_reversed>
inline typename graph_iterator<T, is_const, is_reversed>::const_reference
graph_iterator<T, is_const, is_reversed>::operator*() const {
	return iter->second;
}

template<class T, bool is_const, bool is_reversed>
template <class>
inline typename graph_iterator<T, is_const, is_reversed>::reference
graph_iterator<T, is_const, is_reversed>::operator*() {
	return iter->second;
}

template<class T, bool is_const, bool is_reversed>
template <class>
inline typename graph_iterator<T, is_const, is_reversed>::pointer
graph_iterator<T, is_const, is_reversed>::operator->() {
	return &(iter->second);
}

template<class T, bool is_const, bool is_reversed>
inline typename graph_iterator<T, is_const, is_reversed>::const_pointer
graph_iterator<T, is_const, is_reversed>::operator->() const {
	return &(iter->second);
}

template<class T, bool is_const, bool is_reversed>
inline graph_iterator<T, is_const, is_reversed>& graph_iterator<T, is_const, is_reversed>::operator++() {
	++iter;
	return *this;
}

template<class T, bool is_const, bool is_reversed>
inline graph_iterator<T, is_const, is_reversed> graph_iterator<T, is_const, is_reversed>::operator++(int) {
	return iter++;
}

template<class T, bool is_const, bool is_reversed>
inline graph_iterator<T, is_const, is_reversed>& graph_iterator<T, is_const, is_reversed>::operator--() {
	--iter;
	return *this;
}

template<class T, bool is_const, bool is_reversed>
inline graph_iterator<T, is_const, is_reversed> graph_iterator<T, is_const, is_reversed>::operator--(int) {
	return iter--;
}

template<class T_, bool is_const_, bool is_reversed_>
inline bool operator==(const graph_iterator<T_, is_const_, is_reversed_>& lhs, const graph_iterator<T_, is_const_, is_reversed_>& rhs) {
	return lhs.iter == rhs.iter;
}

template<class T_, bool is_const_, bool is_reversed_>
inline bool operator!=(const graph_iterator<T_, is_const_, is_reversed_>& lhs, const graph_iterator<T_, is_const_, is_reversed_>& rhs) {
	return !operator==(lhs, rhs);
}
