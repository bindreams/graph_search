#pragma once
#include <unordered_set>

#include "json.hpp"
using json = nlohmann::json;

template <class T>
class node {
	template <class T_>
	friend class graph_impl;
private:
	size_t id = 0;

public:
	T value;
	std::unordered_set<node*> edges;

	size_t get_id() const;

	constexpr node() = default;
	node(const T& value_, size_t id_ = 0);
	~node();

	template <class T_>
	friend void from_json(const json& j, node<T_>& obj);
};

template<class T>
inline size_t node<T>::get_id() const {
	return id;
}

template<class T>
inline node<T>::node(const T& value_, size_t id_) :
	id (id_),
	value(value_) {
}

template<class T>
inline node<T>::~node() {
	for (auto&& connected_node : edges) {
		connected_node->edges.erase(this);
	}
}

template<class T>
struct node_equal_values {
	inline bool operator() (const node<T>* const& lhs, const node<T>* const& rhs) const {
		return lhs->value == rhs->value;
	}
};

template<class T>
struct node_numerical_order {
	inline bool operator() (const node<T>* const& lhs, const node<T>* const& rhs) const {
		return lhs->value < rhs->value ||
			(lhs->value == rhs->value && lhs->get_id() < rhs->get_id());
	}
};

template <class T>
std::ostream& operator<<(std::ostream& os, const node<T>& obj) {
	os << "{#" << obj.get_id() << ": " << obj.value << "}";
	return os;
}

template <class T>
void to_json(json& j, const node<T>& obj) {
	j["value"] = obj.value;
	j["id"] = obj.get_id();
	
	for (auto&& i : obj.edges) {
		j["edges"].push_back(i->get_id());
	}
}

template <class T>
void from_json(const json& j, node<T>& obj) {
	obj.value = j["value"].get<T>();
	obj.id = j["id"];

	obj.edges.clear();
}