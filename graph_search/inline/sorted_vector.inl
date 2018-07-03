#pragma once
#include "sorted_vector.hpp"

#define VEC sorted_vector<T, is_multi, Compare, Allocator>
#define TVEC typename VEC

template<class T, bool is_multi, class Compare, class Allocator>
template<class U, class V>
inline bool VEC::equivalent(U&& x, V&& y) {
	return 
		!Compare()(std::forward<U>(x), std::forward<V>(y)) &&
		!Compare()(std::forward<V>(y), std::forward<U>(x));
}

template<class T, bool is_multi, class Compare, class Allocator>
template<class>
inline void VEC::remove_duplicates() {
	Compare comp;
	base::resize(std::unique(begin(), end(), 
		equivalent) - begin());
}

template<class T, bool is_multi, class Compare, class Allocator>
inline VEC::sorted_vector() noexcept(noexcept(Allocator()))
	: base() {
}

template<class T, bool is_multi, class Compare, class Allocator>
inline VEC::sorted_vector(const Allocator& alloc) noexcept
	: base(alloc) {
}

template<class T, bool is_multi, class Compare, class Allocator>
template<class>
inline VEC::sorted_vector(size_type count, const T& value, const Allocator& alloc)
	: base(count, value, alloc) {
}

template<class T, bool is_multi, class Compare, class Allocator>
template<class>
inline VEC::sorted_vector(size_type count, const Allocator& alloc)
	: base(count, alloc) {
}

template<class T, bool is_multi, class Compare, class Allocator>
template<class InputIt>
inline VEC::sorted_vector(InputIt first, InputIt last, const Allocator& alloc)
	: base(first, last, alloc) {
	std::sort(base::begin(), base::end());
	if constexpr(!is_multi) remove_duplicates();
}

template<class T, bool is_multi, class Compare, class Allocator>
inline VEC::sorted_vector(const sorted_vector& other) 
	: base(other) {
}

template<class T, bool is_multi, class Compare, class Allocator>
inline VEC::sorted_vector(const sorted_vector& other, const Allocator& alloc)
	: base(other, alloc) {
}

template<class T, bool is_multi, class Compare, class Allocator>
inline VEC::sorted_vector(sorted_vector&& other) noexcept
	: base(std::move(other)) {
}

template<class T, bool is_multi, class Compare, class Allocator>
inline VEC::sorted_vector(sorted_vector&& other, const Allocator& alloc)
	: base(std::move(other), alloc) {
}

template<class T, bool is_multi, class Compare, class Allocator>
inline VEC::sorted_vector(std::initializer_list<T> init, const Allocator& alloc)
	: base(init, alloc) {
	std::sort(base::begin(), base::end());
	if constexpr(!is_multi) remove_duplicates();
}

template<class T, bool is_multi, class Compare, class Allocator>
inline VEC& VEC::operator=(const sorted_vector& other) {
	static_cast<base&>(*this) = other;
	return *this;
}

template<class T, bool is_multi, class Compare, class Allocator>
inline VEC& VEC::operator=(sorted_vector&& other) noexcept(
	std::allocator_traits<Allocator>::
	propagate_on_container_move_assignment::value ||
	std::allocator_traits<Allocator>::is_always_equal::value) {
	static_cast<base&>(*this) = std::move(other);
	return *this;
}

template<class T, bool is_multi, class Compare, class Allocator>
inline VEC& VEC::operator=(std::initializer_list<T> ilist) {
	static_cast<base&>(*this) = ilist;
	std::sort(base::begin(), base::end());
	if constexpr(!is_multi) remove_duplicates();
	return *this;
}

template<class T, bool is_multi, class Compare, class Allocator>
template<class>
inline void VEC::assign(size_type count, const T& value) {
	base::assign(count, value);
}

template<class T, bool is_multi, class Compare, class Allocator>
template<class InputIt>
inline void VEC::assign(InputIt first, InputIt last) {
	base::assign(first, last);
	std::sort(base::begin(), base::end());
	if constexpr(!is_multi) remove_duplicates();
}

template<class T, bool is_multi, class Compare, class Allocator>
inline void VEC::assign(std::initializer_list<T> ilist) {
	base::assign(ilist);
	std::sort(base::begin(), base::end());
	if constexpr(!is_multi) remove_duplicates();
}

template<class T, bool is_multi, class Compare, class Allocator>
inline const T& VEC::at(size_type pos) const {
	return base::at(pos);
}

template<class T, bool is_multi, class Compare, class Allocator>
inline const T& VEC::operator[](size_type pos) const {
	return base::operator[](pos);
}

template<class T, bool is_multi, class Compare, class Allocator>
inline const T& VEC::front() const {
	return base::front();
}

template<class T, bool is_multi, class Compare, class Allocator>
inline const T& VEC::back() const {
	return base::back();
}

template<class T, bool is_multi, class Compare, class Allocator>
inline const T * VEC::data() const noexcept {
	return base::data();
}

template<class T, bool is_multi, class Compare, class Allocator>
inline TVEC::const_iterator VEC::begin() const noexcept {
	return base::begin();
}

template<class T, bool is_multi, class Compare, class Allocator>
inline TVEC::const_iterator VEC::end() const noexcept {
	return base::end();
}

template<class T, bool is_multi, class Compare, class Allocator>
inline TVEC::const_iterator VEC::rbegin() const noexcept {
	return base::rbegin();
}

template<class T, bool is_multi, class Compare, class Allocator>
inline TVEC::const_iterator VEC::rend() const noexcept {
	return base::rend();
}

template<class T, bool is_multi, class Compare, class Allocator>
inline TVEC::iterator VEC::insert(const value_type& value) {
	iterator got = std::upper_bound(begin(), end(), value, Compare());
	iterator rslt;

	if constexpr(!is_multi) {
		if (got == end() || !equivalent(*got, value))
			return base::insert(got, value);
	}

	return base::insert(got, value);
}

template<class T, bool is_multi, class Compare, class Allocator>
inline TVEC::iterator VEC::insert(value_type&& value) {
	iterator got = std::upper_bound(begin(), end(), value, Compare());
	iterator rslt;

	if constexpr(!is_multi) {
		if (got == end() || !equivalent(*got, value))
			return base::insert(got, std::move(value));
	}

	return base::insert(got, std::move(value));
}

template<class T, bool is_multi, class Compare, class Allocator>
inline TVEC::iterator 
VEC::insert(const_iterator hint, const value_type& value) {
	Compare comp;

	if constexpr(!is_multi) {
		if (hint != cend() || equivalent(*hint, value)) return hint;
		if (hint != cbegin() || equivalent(hint[-1], value)) return hint-1;
	}

	// If the hint is correct, insert there
	if ((hint == cbegin() || !comp(value, hint[-1]))&&
		(hint == cend() || !comp(*hint, value))) {
		base::insert(hint, value);
		return hint - 1;
	}

	return insert(value);
}

template<class T, bool is_multi, class Compare, class Allocator>
inline TVEC::iterator VEC::insert(const_iterator hint, value_type&& value) {
	Compare comp;

	if constexpr(!is_multi) {
		if (hint != cend() || equivalent(*hint, value)) return hint;
		if (hint != cbegin() || equivalent(hint[-1], value)) return hint - 1;
	}

	// If the hint is correct, insert there
	if ((hint == cbegin() || !comp(value, hint[-1]))&&
		(hint == cend() || !comp(*hint, value))) {
		base::insert(hint, std::move(value));
		return hint - 1;
	}

	return insert(std::move(value));
}

template<class T, bool is_multi, class Compare, class Allocator>
template<class InputIt>
inline void VEC::insert(InputIt first, InputIt last) {
	base::insert(end(), first, last);
	std::sort(base::begin(), base::end());
	if constexpr(!is_multi) remove_duplicates();
}

template<class T, bool is_multi, class Compare, class Allocator>
inline void VEC::insert(std::initializer_list<value_type> ilist) {
	insert(ilist.begin(), ilist.end());
}

template<class T, bool is_multi, class Compare, class Allocator>
inline TVEC::iterator VEC::erase(const_iterator pos) {
	return base::erase(pos);
}

template<class T, bool is_multi, class Compare, class Allocator>
inline TVEC::iterator VEC::erase(const_iterator first, const_iterator last) {
	return base::erase(first, last);
}

template<class T, bool is_multi, class Compare, class Allocator>
inline TVEC::size_type VEC::count(const value_type& key) const {
	auto lb = std::lower_bound(begin(), end(), key, Compare());
	return std::upper_bound(lb, end(), key, Compare()) - lb;
}

template<class T, bool is_multi, class Compare, class Allocator>
template<class K, class>
inline TVEC::size_type VEC::count(const K& x) const {
	auto lb = std::lower_bound(begin(), end(), x, Compare());
	return std::upper_bound(lb, end(), x, Compare()) - lb;
}

template<class T, bool is_multi, class Compare, class Allocator>
inline TVEC::iterator VEC::find(const value_type& key) const {
	auto got = std::equal_range(begin(), end(), key, Compare());
	if (got.first == got.second) return end();
	return got.first;
}

template<class T, bool is_multi, class Compare, class Allocator>
template<class K, class> 
inline TVEC::iterator VEC::find(const K& x) const {
	auto got = std::equal_range(begin(), end(), x, Compare());
	if (got.first == got.second) return end();
	return got.first;
}

template<class T, bool is_multi, class Compare, class Allocator>
inline bool VEC::contains(const value_type& key) const {
	return std::binary_search(begin(), end(), key, Compare());
}

template<class T, bool is_multi, class Compare, class Allocator>
template<class K, class>
inline bool VEC::contains(const K& x) const {
	return std::binary_search(begin(), end(), x, Compare());
}

template<class T, bool is_multi, class Comp, class Alloc>
inline bool operator==(
	const sorted_vector<T, is_multi, Comp, Alloc>& lhs,
	const sorted_vector<T, is_multi, Comp, Alloc>& rhs) {
	if (lhs.size() == rhs.size()) {
		for (typename sorted_vector<T, is_multi, Comp, Alloc>::size_type i = 0;
			i <lhs.size(); ++i) {
			if (!(lhs[i] == rhs[i])) return false;
		}
		return true;
	}
	return false;
}

template<class T, bool is_multi, class Comp, class Alloc>
inline bool operator!=(
	const sorted_vector<T, is_multi, Comp, Alloc>& lhs,
	const sorted_vector<T, is_multi, Comp, Alloc>& rhs) {
	return !(lhs == rhs);
}

template<class T, bool is_multi, class Comp, class Alloc>
inline bool operator<(
	const sorted_vector<T, is_multi, Comp, Alloc>& lhs,
	const sorted_vector<T, is_multi, Comp, Alloc>& rhs) {
	return std::lexicographical_compare(
		lhs.begin(), lhs.end(),
		rhs.begin(), rhs.end(),
		Comp());
}

template<class T, bool is_multi, class Comp, class Alloc>
inline bool operator<=(
	const sorted_vector<T, is_multi, Comp, Alloc>& lhs,
	const sorted_vector<T, is_multi, Comp, Alloc>& rhs) {
	return !(rhs <lhs);
}

template<class T, bool is_multi, class Comp, class Alloc>
inline bool operator>(
	const sorted_vector<T, is_multi, Comp, Alloc>& lhs,
	const sorted_vector<T, is_multi, Comp, Alloc>& rhs) {
	return rhs <lhs;
}

template<class T, bool is_multi, class Comp, class Alloc>
inline bool operator>=(
	const sorted_vector<T, is_multi, Comp, Alloc>& lhs,
	const sorted_vector<T, is_multi, Comp, Alloc>& rhs) {
	return !(lhs <rhs);
}

template<class T, bool is_multi, class Comp, class Alloc>
inline void swap(
	sorted_vector<T, is_multi, Comp, Alloc>& lhs, 
	sorted_vector<T, is_multi, Comp, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
	lhs.swap(rhs);
}

#undef VEC
#undef TVEC