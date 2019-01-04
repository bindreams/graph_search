#pragma once
#include <tuple>
#include "container_view_impl.hpp"
#include "type_traits/has_size.hpp"
#include "type_traits/is_bidirectional.hpp"
#include "type_traits/is_random_access.hpp"

namespace zh {
namespace detail {

// Specialized container_view_impl with no reverse_iterators.
template <
	class Container,
	class Iterator,
	class ConstIterator,
	class ReverseIterator,      // not used
	class ConstReverseIterator, // not used
	class... Args>
class container_view_impl<
	Container,
	Iterator,
	ConstIterator,
	ReverseIterator,
	ConstReverseIterator,
	false,
	Args...>
	: std::tuple<Args...> {
private:
	Container& c;

protected:
	using container_type = Container;
	using iterator_args = std::tuple<Args...>;
	
	constexpr container_type& base() noexcept;
	constexpr const container_type& base() const noexcept;

	constexpr iterator_args& args() noexcept;
	constexpr const iterator_args& args() const noexcept;

public:
	// Member types ===========================================================
	using value_type      = typename std::iterator_traits<Iterator>     ::value_type;
	using difference_type = typename std::iterator_traits<Iterator>     ::difference_type;
	using reference       = typename std::iterator_traits<Iterator>     ::reference;
	using const_reference = typename std::iterator_traits<ConstIterator>::reference;
	using pointer         = typename std::iterator_traits<Iterator>     ::pointer;
	using const_pointer   = typename std::iterator_traits<ConstIterator>::pointer;

	using iterator = Iterator;
	using const_iterator = ConstIterator;

	// Member functions =======================================================
	// Constructors -----------------------------------------------------------
	constexpr container_view_impl(Container& container);

	// Construct with container and store additional arguments for constructing
	// iterators in a tuple.
	template <class... IteratorArgs>
	constexpr container_view_impl(Container& container, IteratorArgs&&... args);

	~container_view_impl() = default;

	// Iterators --------------------------------------------------------------
	constexpr Iterator begin() noexcept;
	constexpr ConstIterator begin() const noexcept;
	constexpr ConstIterator cbegin() const noexcept;

	constexpr Iterator end() noexcept;
	constexpr ConstIterator end() const noexcept;
	constexpr ConstIterator cend() const noexcept;

	// Observers --------------------------------------------------------------
	template <class = std::enable_if_t<has_size_v<Container>>>
	constexpr std::size_t size() const noexcept;

	template <class = std::enable_if_t<has_size_v<Container>>>
	constexpr bool empty() const noexcept;

	// Element access ---------------------------------------------------------
	constexpr reference front();

	constexpr const_reference front() const;

	template <class Dummy = int, class = std::enable_if_t<sizeof(Dummy) != 0 && 
		is_bidirectional_v<Iterator>>>
	constexpr reference back();

	template <class Dummy = int, class = std::enable_if_t<sizeof(Dummy) != 0 && 
		is_bidirectional_v<ConstIterator>>>
	constexpr const_reference back() const;

	template <class Dummy = int, class = std::enable_if_t<sizeof(Dummy) != 0 && 
		is_random_access_v<Iterator>>>
	constexpr reference operator[](std::size_t index);

	template <class Dummy = int, class = std::enable_if_t<sizeof(Dummy) != 0 && 
		is_random_access_v<ConstIterator>>>
	constexpr const_reference operator[](std::size_t index) const;
};

} // namespace detail
} // namespace zh

#include "container_view_impl_no_reverse.inl"