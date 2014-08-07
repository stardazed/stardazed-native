// ------------------------------------------------------------------
// container::RefTree - stardazed
// (c) 2014 by Arthur Langereis
// ------------------------------------------------------------------

#ifndef SD_CONTAINER_REFTREE_H
#define SD_CONTAINER_REFTREE_H

#include "container/ObjectPool.hpp"
#include <vector>

namespace stardazed {
namespace container {
		

template <typename T, size_t PoolChunkCount = 512>
class RefTree {
	class Node {
		T* item_;
		std::vector<Node*> children_;
		
	public:
		Node(T* item) : item_{item} {}

		// it is UB to get the item of the root node
		T& item() const { return *item_; }
		
		size_t size() const { return children_.size(); }
		bool empty() const { return children_.empty(); }
		void reserve(size_t n) { return children_.reserve(n); }
		
		void append(Node* node) {
			children_.push_back(node);
		}
		
		auto begin() { return children_.begin(); }
		auto end() { return children_.end(); }
		
		using Iterator = typename decltype(children_)::iterator;
	};

	container::ObjectPoolChain<Node, PoolChunkCount> nodePool_;
	Node* root_;

public:
	RefTree() : root_(nodePool_.emplace(nullptr)) {}
	
	Node& root() const { return *root_; }

	Node* makeNode(T& item) {
		return nodePool_.emplace(&item);
	}
	
	using Iterator = typename Node::Iterator;
};
		
		
} // ns container
} // ns stardazed

#endif
