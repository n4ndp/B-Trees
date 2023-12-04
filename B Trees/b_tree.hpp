#pragma once
#include <memory>
#include <vector>
// Cormen et al. "Introduction to Algorithms" 3rd ed. (2009)

// t is the minimum degree of the tree (t >= 2)
// Every node other than the root must have at least t - 1 keys.
// Every internal node other than the root must have at least t children.
// Every node may contain at most 2t - 1 keys.
// Therefore, an internal node may have at most 2t children.
// We say that a node is full if it contains exactly 2t - 1 keys.

template <typename TK, typename TV>
class BTree {
private:
    struct Node {
        bool leaf; // is leaf?
        
        std::shared_ptr<Node> parent;
        std::vector<std::shared_ptr<Node>> children;
        std::vector<std::pair<TK, std::unique_ptr<TV>>> keys;
    };
    
    size_t t; // minimum degree of the tree
    std::shared_ptr<Node> root; // root of the tree

public:
    

};


