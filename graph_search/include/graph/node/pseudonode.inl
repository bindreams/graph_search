#pragma once
#include "pseudonode.hpp"

template<class T>
inline pseudonode<T>::operator node<T>&() noexcept {
	return m_imitator;
}

template<class T>
inline pseudonode<T>::operator const node<T>&() const noexcept {
	return m_imitator;
}

template<class T>
inline node<T>& pseudonode<T>::imitator() noexcept {
	return m_imitator;
}

template<class T>
inline const node<T>& pseudonode<T>::imitator() const noexcept {
	return m_imitator;
}

// This ctor assumes that the number, returned by id() is the pointer to
// the internal T value.
template<class T>
inline pseudonode<T>::pseudonode(const graph<T>& gr, typename node<T>::id_type id) :
	m_imitator(const_cast<graph<T>&>(gr), reinterpret_cast<T*>(id)) {
}

template<class T>
inline pseudonode<T>::~pseudonode() {
	// Release the stored value to prevent destruction.
	// Note: no memory leaks occur. Pseudonode is assumed to hold imitators to
	// alreasy existing ids.
	m_imitator.m_value.release();
}