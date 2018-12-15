#pragma once
#include <iterator>
#include <type_traits>

namespace zh {

template <class Iterator, class ConstIterator>
class container_view {
private:
	Iterator m_begin;
	Iterator m_end;
	ConstIterator m_cbegin;
	ConstIterator m_cend;

public:
	// Member types ===========================================================
	using value_type = typename
		std::iterator_traits<Iterator>::value_type;
	using difference_type = typename
		std::iterator_traits<Iterator>::difference_type;
	using reference = typename
		std::iterator_traits<Iterator>::reference;
	using const_reference = typename
		std::iterator_traits<ConstIterator>::reference;
	using pointer = typename
		std::iterator_traits<Iterator>::pointer;
	using const_pointer = typename
		std::iterator_traits<ConstIterator>::pointer;

	static_assert(!std::is_same_v<Iterator, ConstIterator>,
		"container_view: Iterator and ConstIterator cannot be the same. Use const_container_view instead");
	static_assert(!std::is_same_v<const const_reference, reference>,
		"container_view: Iterator cannot be const version of ConstIterator");

	using iterator = Iterator;
	using const_iterator = ConstIterator;

	// Member functions =======================================================
	// Constructors -----------------------------------------------------------
	constexpr container_view() = default;
	constexpr container_view(
		Iterator first,
		Iterator last,
		ConstIterator cfirst,
		ConstIterator clast) :
		m_begin(first),
		m_end(last),
		m_cbegin(cfirst),
		m_cend(clast) {
	}
	~container_view() = default;

	// Iterators --------------------------------------------------------------
	Iterator begin() noexcept { return m_begin; }
	ConstIterator begin() const noexcept { return m_cbegin; }
	ConstIterator cbegin() const noexcept { return m_cbegin; }

	Iterator end() noexcept { return m_end; }
	ConstIterator end() const noexcept { return m_cend; }
	ConstIterator cend() const noexcept { return m_cend; }

	// Observers --------------------------------------------------------------
	std::size_t size() const noexcept { 
		return std::distance(m_cbegin, m_cend); 
	}

	bool empty() const noexcept {
		return m_cbegin == m_cend;
	}
};

template <class ConstIterator>
class const_container_view {
private:
	ConstIterator m_cbegin;
	ConstIterator m_cend;

public:
	// Member types ===========================================================
	using value_type = typename
		std::iterator_traits<ConstIterator>::value_type;
	using difference_type = typename
		std::iterator_traits<ConstIterator>::difference_type;
	using reference = typename
		std::iterator_traits<ConstIterator>::reference;
	using const_reference = typename
		std::iterator_traits<ConstIterator>::reference;
	using pointer = typename
		std::iterator_traits<ConstIterator>::pointer;
	using const_pointer = typename
		std::iterator_traits<ConstIterator>::pointer;

	using iterator = ConstIterator;
	using const_iterator = ConstIterator;

	// Member functions =======================================================
	// Constructors -----------------------------------------------------------
	constexpr const_container_view() = default;
	constexpr const_container_view(
		ConstIterator cfirst,
		ConstIterator clast) :
		m_cbegin(cfirst),
		m_cend(clast) {
	}

	// Construct from a mutable container_view
	template <class Iterator>
	constexpr const_container_view(
		const container_view<Iterator, ConstIterator>& view) :
		m_cbegin(view.cbegin()),
		m_cend(view.cend()) {
	}

	~const_container_view() = default;

	// Iterators --------------------------------------------------------------
	ConstIterator begin() const noexcept { return m_cbegin; }
	ConstIterator cbegin() const noexcept { return m_cbegin; }

	ConstIterator end() const noexcept { return m_cend; }
	ConstIterator cend() const noexcept { return m_cend; }

	// Observers --------------------------------------------------------------
	std::size_t size() const noexcept {
		return std::distance(m_cbegin, m_cend);
	}

	bool empty() const noexcept {
		return m_cbegin == m_cend;
	}
};

template <class Iterator, class ConstIterator>
container_view<Iterator, ConstIterator> make_container_view(
	Iterator first,
	Iterator last,
	ConstIterator cfirst,
	ConstIterator clast) {
	return container_view<Iterator, ConstIterator>(
		first,
		last,
		cfirst,
		clast);
}

template <class ConstIterator>
const_container_view<ConstIterator> make_container_view(
	ConstIterator cfirst,
	ConstIterator clast) {
	return const_container_view<ConstIterator>(
		cfirst,
		clast);
}

} // namespace zh