#pragma once
#include <vector>
#include <algorithm>
#include <type_traits>

template <
	class T,
	bool is_multi = true,
	class Compare = std::less<T>, 
	class Allocator = std::allocator<T>>
class sorted_vector : private std::vector<T, Allocator> {
private:
	using base = std::vector<T, Allocator>;

	template <class U, class V>
	bool equivalent(U&& x, V&& y);

	template<class = std::enable_if_t<!is_multi>>
	void remove_duplicates();

public:
	// Member types ============================================================
	using value_type             = typename base::value_type;
	using value_compare          = Compare;
	using allocator_type         = typename base::allocator_type;
	using size_type              = typename base::size_type;
	using difference_type        = typename base::difference_type;
	using reference              = typename base::reference;
	using const_reference        = typename base::const_reference;
	using pointer                = typename base::pointer;
	using const_pointer          = typename base::const_pointer;
	using iterator               = typename base::const_iterator;
	using const_iterator         = typename base::const_iterator;
	using reverse_iterator       = typename base::const_reverse_iterator;
	using const_reverse_iterator = typename base::const_reverse_iterator;

	// Member functions ========================================================
	// Constructors ------------------------------------------------------------
	sorted_vector() noexcept(noexcept(Allocator()));
	explicit sorted_vector(const Allocator& alloc) noexcept;

	template<class = std::enable_if_t<is_multi>>
	sorted_vector(
		size_type count,
		const T& value,
		const Allocator& alloc = Allocator());

	template<class = std::enable_if_t<is_multi>>
	explicit sorted_vector(
		size_type count, 
		const Allocator& alloc = Allocator());

	template<class InputIt>
	sorted_vector(
		InputIt first, InputIt last,
		const Allocator& alloc = Allocator());

	sorted_vector(const sorted_vector& other);
	sorted_vector(const sorted_vector& other, const Allocator& alloc);
	sorted_vector(sorted_vector&& other) noexcept;
	sorted_vector(sorted_vector&& other, const Allocator& alloc);
	sorted_vector(std::initializer_list<T> init,
		const Allocator& alloc = Allocator());

	// Destructor --------------------------------------------------------------
	~sorted_vector() = default;

	// Assignment --------------------------------------------------------------
	sorted_vector& operator=(const sorted_vector& other);
	sorted_vector& operator=(sorted_vector&& other) noexcept(
		std::allocator_traits<Allocator>::
		propagate_on_container_move_assignment::value
		|| std::allocator_traits<Allocator>::is_always_equal::value);
	sorted_vector& operator=(std::initializer_list<T> ilist);

	template <class = std::enable_if_t<is_multi>>
	void assign(size_type count, const T& value);
	template<class InputIt>
	void assign(InputIt first, InputIt last);
	void assign(std::initializer_list<T> ilist);

	using base::get_allocator;

	// Element access ----------------------------------------------------------
	const T& at(size_type pos) const;
	const T& operator[](size_type pos) const;

	const T& front() const;
	const T& back() const;

	const T* data() const noexcept;

	// Iterators ---------------------------------------------------------------
	const_iterator begin() const noexcept;
	using base::cbegin;

	const_iterator end() const noexcept;
	using base::cend;

	const_iterator rbegin() const noexcept;
	using base::crbegin;

	const_iterator rend() const noexcept;
	using base::crend;

	// Capacity ----------------------------------------------------------------
	using base::empty;
	using base::size;
	using base::max_size;
	using base::reserve;
	using base::capacity;
	using base::shrink_to_fit;

	// Modifiers ---------------------------------------------------------------
	using base::clear;

	iterator insert(const value_type& value);
	iterator insert(value_type&& value);
	iterator insert(const_iterator hint, const value_type& value);
	iterator insert(const_iterator hint, value_type&& value);
	template<class InputIt>
	void insert(InputIt first, InputIt last);
	void insert(std::initializer_list<value_type> ilist);

	iterator erase(const_iterator pos);
	iterator erase(const_iterator first, const_iterator last);

	using base::swap;

	// Lookup ------------------------------------------------------------------
	size_type count(const value_type& key) const;
	template<class K, class = typename Compare::is_transparent>
	size_type count(const K& x) const;

	iterator find(const value_type& key) const;
	template<class K, class = typename Compare::is_transparent> 
	iterator find(const K& x) const;

	bool contains(const value_type& key) const;
	template<class K, class = typename Compare::is_transparent>
	bool contains(const K& x) const;
};

template<class T, bool is_multi, class Comp, class Alloc>
inline bool operator==(
	const sorted_vector<T, is_multi, Comp, Alloc>& lhs,
	const sorted_vector<T, is_multi, Comp, Alloc>& rhs);

template<class T, bool is_multi, class Comp, class Alloc>
inline bool operator!=(
	const sorted_vector<T, is_multi, Comp, Alloc>& lhs,
	const sorted_vector<T, is_multi, Comp, Alloc>& rhs);

template<class T, bool is_multi, class Comp, class Alloc>
inline bool operator<(
	const sorted_vector<T, is_multi, Comp, Alloc>& lhs,
	const sorted_vector<T, is_multi, Comp, Alloc>& rhs);

template<class T, bool is_multi, class Comp, class Alloc>
inline bool operator<=(
	const sorted_vector<T, is_multi, Comp, Alloc>& lhs,
	const sorted_vector<T, is_multi, Comp, Alloc>& rhs);

template<class T, bool is_multi, class Comp, class Alloc>
inline bool operator>(
	const sorted_vector<T, is_multi, Comp, Alloc>& lhs,
	const sorted_vector<T, is_multi, Comp, Alloc>& rhs);

template<class T, bool is_multi, class Comp, class Alloc>
inline bool operator>=(
	const sorted_vector<T, is_multi, Comp, Alloc>& lhs,
	const sorted_vector<T, is_multi, Comp, Alloc>& rhs);

template<class T, bool is_multi, class Comp, class Alloc>
void swap(
	sorted_vector<T, is_multi, Comp, Alloc>& lhs,
	sorted_vector<T, is_multi, Comp, Alloc>& rhs) noexcept(noexcept(lhs.swap(rhs)));

#include "inline/sorted_vector.inl"
