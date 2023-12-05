#pragma once
#include <memory>
#include <vector>
#include <utility>
#include <iostream>

// t is the minimum degree of the tree (t >= 2)
// The difference between B+ tree and B tree is that
// B+ tree only stores keys in the leaf nodes.
// All the keys in the internal nodes are used for indexing.
// Therefore, the number of keys in the internal nodes is
// always one less than the number of children.
// Every node other than the root must have at least t - 1 keys.
// Every internal node other than the root must have at least t children.
// Every node may contain at most 2t - 1 keys.
// Therefore, an internal node may have at most 2t children.
// We say that a node is full if it contains exactly 2t - 1 keys.

template <typename TK>
class BPlusTree {
private:
};