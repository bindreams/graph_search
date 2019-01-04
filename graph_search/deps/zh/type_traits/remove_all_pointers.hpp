#pragma once
#include <type_traits>

namespace zh {

template <class T>
struct remove_all_pointers {
	using type = T;
};

template <class T>
struct remove_all_pointers<T*> {
	using type = typename remove_all_pointers<T>::type;
};

template <class T>
struct remove_all_pointers<T* const> {
	using type = typename remove_all_pointers<T>::type;
};

template <class T>
struct remove_all_pointers<T* volatile> {
	using type = typename remove_all_pointers<T>::type;
};

template <class T> 
struct remove_all_pointers<T* const volatile> {
	using type = typename remove_all_pointers<T>::type;
};

template <class T>
using remove_all_pointers_t = typename remove_all_pointers<T>::type;

} // namespace zh