#pragma once
#include <iterator>
#include <type_traits>

#define ITER_DECL(...) \
	template< \
	class BaseIterator_, class FunctorType_, \
		class = typename proxy_iterator<BaseIterator_, FunctorType_>:: \
		enable_for<__VA_ARGS__>>

#define ITER proxy_iterator<BaseIterator_, FunctorType_>

template <class BaseIterator, class FunctorType>
class proxy_iterator : private FunctorType {
private:
	BaseIterator iter;

public:
	BaseIterator& get_iterator() {
		return iter;
	}

	const BaseIterator& get_iterator() const {
		return iter;
	}

	// Member types ============================================================
	using value_type 
		= typename std::remove_reference<decltype(FunctorType()(*iter))>::type;
	using difference_type   
		= typename std::iterator_traits<BaseIterator>::difference_type;
	using pointer           
		= value_type*;
	using reference         
		= value_type&;
	using iterator_category 
		= typename std::iterator_traits<BaseIterator>::iterator_category;

private:
	template <class... Types>
	using enable_for = typename std::enable_if<std::conjunction<
		std::is_base_of<Types, iterator_category>...>::value>::type;

public:
	// Member functions ========================================================
	// Required by RandomAccessIterator ----------------------------------------
	template <class = enable_for<std::random_access_iterator_tag>>
	decltype(auto) operator[](difference_type offset) const {
		return FunctorType::operator()(iter[offset]);
	}

	template <class = enable_for<std::random_access_iterator_tag>>
	proxy_iterator& operator+=(difference_type offset) {
		iter += offset;
		return *this;
	}

	template <class = enable_for<std::random_access_iterator_tag>>
	proxy_iterator& operator-=(difference_type offset) {
		iter -= offset;
		return *this;
	}

	// Required by BidirectionalIterator ---------------------------------------
	template <class = enable_for<std::bidirectional_iterator_tag>>
	proxy_iterator& operator--() {
		--iter;
		return *this;
	}

	template <class = enable_for<std::bidirectional_iterator_tag>>
	proxy_iterator operator--(int) {
		proxy_iterator temp = *this;
		iter--;
		return temp;
	}

	// Required by ForwardIterator ---------------------------------------------
	template <class = enable_for<std::forward_iterator_tag>>
	proxy_iterator() :
		iter() {
	}

	// Required by InputIterator -----------------------------------------------
	template <class = enable_for<std::input_iterator_tag>>
	decltype(auto) operator->() const {
		return &FunctorType::operator()(*iter);
	}

	// Always required ---------------------------------------------------------
	proxy_iterator& operator++() {
		++iter;
		return *this;
	}

	proxy_iterator operator++(int) {
		proxy_iterator temp = *this;
		iter++;
		return temp;
	}

	decltype(auto) operator*() const {
		return FunctorType::operator()(*iter);
	}

	proxy_iterator(const proxy_iterator&) = default;
	proxy_iterator(proxy_iterator&&) = default;

	explicit proxy_iterator(const BaseIterator& other) :
		iter(other) {
	}

	// Convert from related proxy iterators
	// Example: proxy_iterator<iterator, ...> to proxy_iterator<const_iterator, ...>
	template<class Iter, class = std::enable_if<std::is_convertible<Iter, BaseIterator>::value>::type>
	proxy_iterator(const proxy_iterator<Iter, FunctorType>& other) :
		iter(other.iter) {
	}

	proxy_iterator& operator=(const proxy_iterator&) = default;
	proxy_iterator& operator=(proxy_iterator&&) = default;
	proxy_iterator& operator=(const BaseIterator& other) {
		iter = other;
	}

	// Friends -----------------------------------------------------------------
	template <class BaseIterator_, class FunctorType_>
	friend class proxy_iterator;

	// Required by RandomAccessIterator ----------------------------------------
	ITER_DECL(std::random_access_iterator_tag)
		friend inline ITER operator+(ITER iter, typename ITER::difference_type offset);

	ITER_DECL(std::random_access_iterator_tag)
		friend inline ITER operator+(typename ITER::difference_type offset, ITER iter);

	ITER_DECL(std::random_access_iterator_tag)
		friend inline ITER operator-(ITER iter, typename ITER::difference_type offset);

	ITER_DECL(std::random_access_iterator_tag)
		friend inline typename ITER::difference_type operator-(ITER iter1, ITER iter2);

	ITER_DECL(std::random_access_iterator_tag)
		friend inline decltype(auto) operator< (ITER iter1, ITER iter2);

	ITER_DECL(std::random_access_iterator_tag)
		friend inline decltype(auto) operator> (ITER iter1, ITER iter2);

	ITER_DECL(std::random_access_iterator_tag)
		friend inline decltype(auto) operator<=(ITER iter1, ITER iter2);

	ITER_DECL(std::random_access_iterator_tag)
		friend inline decltype(auto) operator>=(ITER iter1, ITER iter2);

	// Required by InputIterator -----------------------------------------------

	ITER_DECL(std::input_iterator_tag)
		friend inline decltype(auto) operator==(ITER iter1, ITER iter2);

	ITER_DECL(std::input_iterator_tag)
		friend inline decltype(auto) operator!=(ITER iter1, ITER iter2);
};

// Friends -----------------------------------------------------------------
// Required by RandomAccessIterator ----------------------------------------
ITER_DECL(std::random_access_iterator_tag)
inline ITER operator+(ITER iter, typename ITER::difference_type offset) {
	return iter.iter + offset.iter;
}

ITER_DECL(std::random_access_iterator_tag)
inline ITER operator+(typename ITER::difference_type offset, ITER iter) {
	return iter.iter + offset.iter;
}

ITER_DECL(std::random_access_iterator_tag)
inline ITER operator-(ITER iter, typename ITER::difference_type offset) {
	return iter.iter - offset.iter;
}

ITER_DECL(std::random_access_iterator_tag)
inline typename ITER::difference_type operator-(ITER iter1, ITER iter2) {
	return iter1.iter - iter2.iter;
}

ITER_DECL(std::random_access_iterator_tag)
inline decltype(auto) operator< (ITER iter1, ITER iter2) {
	return iter1.iter < iter2.iter;
}

ITER_DECL(std::random_access_iterator_tag)
inline decltype(auto) operator> (ITER iter1, ITER iter2) {
	return iter1.iter > iter2.iter;
}

ITER_DECL(std::random_access_iterator_tag)
inline decltype(auto) operator<=(ITER iter1, ITER iter2) {
	return iter1.iter <= iter2.iter;
}

ITER_DECL(std::random_access_iterator_tag)
inline decltype(auto) operator>=(ITER iter1, ITER iter2) {
	return iter1.iter >= iter2.iter;
}

// Required by InputIterator -----------------------------------------------

ITER_DECL(std::input_iterator_tag)
inline decltype(auto) operator==(ITER iter1, ITER iter2) {
	return iter1.iter == iter2.iter;
}

ITER_DECL(std::input_iterator_tag)
inline decltype(auto) operator!=(ITER iter1, ITER iter2) {
	return iter1.iter != iter2.iter;
}

#undef ITER_DECL
#undef ITER