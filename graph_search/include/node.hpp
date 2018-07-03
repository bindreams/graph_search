#pragma once
#include <unordered_set>
#include <memory>

#include "deps/json.hpp"
using json = nlohmann::json;

template <class T>
class node {
private:
	std::unique_ptr<T> value;

	size_t id = 0;
	std::unordered_set<node*> edges;

public:
	//Constructor
	node(const T& value_, size_t id_ = 0);

	constexpr node() = default;
	node(const node& other) = delete;
	node(node&& other) = default;
	~node();

	node& operator=(const node& rhs) = delete;
	node& operator=(node&& rhs) = default;

	//Member access
	T& operator*();
	const T& operator*() const;
	T* operator->();
	const T* operator->() const;

	//Connecting
	void fw_connect(node* n); //one-way connecting
	void bi_connect(node* n); //two-way connecting

	void fw_disconnect(node* n);
	void bi_disconnect(node* n);

	//Modification
	const std::unordered_set<node*>& get_edges() const;
	size_t get_id() const;

	//Json serialization
	template <class T_>
	friend void from_json(const json& j, node<T_>& obj);
};

template<class T>
inline T& node<T>::operator*() {
	return *value;
}

template<class T>
inline const T & node<T>::operator*() const {
	return *value;
}

template<class T>
inline T * node<T>::operator->() {
	return value.get();
}

template<class T>
inline const T * node<T>::operator->() const {
	return value.get();
}

template<class T>
inline void node<T>::fw_connect(node * n) {
	edges.emplace(n);
}

template<class T>
inline void node<T>::bi_connect(node * n) {
	fw_connect(n);
	n->fw_connect(this);
}

template<class T>
inline void node<T>::fw_disconnect(node * n) {
	edges.erase(n);
}

template<class T>
inline void node<T>::bi_disconnect(node * n) {
	fw_disconnect(n);
	n->fw_disconnect(this);
}

template<class T>
inline const std::unordered_set<node<T>*>& node<T>::get_edges() const {
	return edges;
}

template<class T>
inline size_t node<T>::get_id() const {
	return id;
}

template<class T>
inline node<T>::node(const T& value_, size_t id_) :
	id(id_),
	value(std::make_unique<T>(value_)) {
}

template<class T>
inline node<T>::~node() {
	for (auto&& connected_node : edges) {
		connected_node->edges.erase(this);
	}
}

template<class T>
struct node_value_equal {
	inline bool operator() (const node<T>* const& lhs, const node<T>* const& rhs) const {
		return **lhs == **rhs;
	}
};

template<class T>
struct node_value_compare {
	inline bool operator() (const node<T>* const& lhs, const node<T>* const& rhs) const {
		return **lhs < **rhs;
	}
};

template<class T>
struct node_value_order {
	inline bool operator() (const node<T>* const& lhs, const node<T>* const& rhs) const {
		return **lhs < **rhs ||
			(**lhs == **rhs && lhs->get_id() < rhs->get_id());
	}
};

template <class T>
std::ostream& operator<<(std::ostream& os, const node<T>& obj) {
	os << "{#" << obj.get_id() << ": " << *obj << "}";
	return os;
}

template <class T>
void to_json(json& j, const node<T>& obj) {
	j["value"] = *obj;
	j["id"] = obj.get_id();

	for (auto&& i : obj.get_edges()) {
		j["edges"].push_back(i->get_id());
	}
}

template <class T>
void from_json(const json& j, node<T>& obj) {
	obj.value = std::make_unique<T>(j["value"].get<T>());
	obj.id = j["id"];

	obj.edges.clear();
}