#pragma once
#include <unordered_set>
#include <memory>
#include <iomanip>
#include <cstdint>

#include "deps/json.hpp"
using json = nlohmann::json;

template <class T>
class node {
public:
	// Constructors ============================================================
	template<class... Args>
	node(Args&&... args);

	node() = delete;
	node(const node&) = delete;
	node(node&&) = default;

	node& operator=(const node&) = delete;
	node& operator=(node&&) = default;
	~node();

	// Observers ===============================================================
	std::size_t id() const;

	// Member access ===========================================================
	T& value();
	const T& value() const;

	const std::unordered_set<node*>& edges() const;

	// Friends =================================================================
	template <class T_>
	friend class graph;
private:
	// Connecting ==============================================================
	void fw_connect(node* n); //one-way connecting
	void bi_connect(node* n); //two-way connecting

	void fw_disconnect(node* n);
	void bi_disconnect(node* n);

	// Members =================================================================
	std::unique_ptr<T> m_value;
	std::unordered_set<node*> m_edges;
};

#include "inline/node.inl"