#pragma once
#include <memory>
#include <vector>
#include <utility>
#include <iostream>

// t is the minimum degree of the tree (t >= 2)
// Every node other than the root must have at least t - 1 keys.
// Every internal node other than the root must have at least t children.
// Every node may contain at most 2t - 1 keys.
// Therefore, an internal node may have at most 2t children.
// We say that a node is full if it contains exactly 2t - 1 keys.

template <typename TK>
class BTree {
private:
    struct Node {
        bool leaf; // is leaf?
        size_t t; // minimum degree of the tree
        std::vector<std::shared_ptr<Node>> children;
        std::vector<std::unique_ptr<TK>> keys;

        Node(bool leaf) : leaf(leaf) {}

        void insert_non_full(TK key) {
            size_t index = keys.size() - 1;

            if (leaf) { // if the node is a leaf
                keys.push_back(std::make_unique<TK>(key));
                while (index >= 0 && key < *keys[index]) { // insertion sort
                    std::swap(keys[index], keys[index + 1]);
                    --index;
                }
            } else { // if the node is not a leaf
                // find the child which is going to receive the key
                while (index >= 0 && key < *keys[index]) {
                    --index;
                }
                // if the child is full
                if (children[index + 1]->keys.size() == 2 * t - 1) {
                    split_child(index + 1);
                    // if the key is greater than the middle key of the child
                    if (key > *keys[index + 1]) {
                        ++index;
                    }
                }
                // insert the key to the child
                children[index + 1]->insert_non_full(key);
            }
        }

        void split_child(size_t index) {
            auto current_child = children[index];
            auto new_child = std::make_shared<Node>(current_child->leaf);
            new_child->t = current_child->t;

            // Copy the last t - 1 keys of current_child to new_child
            for (size_t j = 0; j < t - 1; ++j) {
                new_child->keys.push_back(std::move(current_child->keys.back()));
                current_child->keys.pop_back();
            }

            if (!current_child->leaf) { // if current_child is not a leaf
                // Copy the last t children of current_child to new_child
                for (size_t j = 0; j < t; ++j) {
                    new_child->children.push_back(std::move(current_child->children.back()));
                    current_child->children.pop_back();
                }
            }

            // Insert the middle key of current_child to this node
            keys.insert(keys.begin() + index, std::move(current_child->keys.back()));
            current_child->keys.pop_back();

            // Insert new_child as a child of this node
            children.insert(children.begin() + index + 1, new_child);
        }
    };
    
    size_t t; // minimum degree of the tree
    std::shared_ptr<Node> root; // root of the tree

public:
    BTree(size_t t) : t(t), root(nullptr) {}

    void insert(TK key) {
        if (root == nullptr) { // if the tree is empty
            root = std::make_shared<Node>(true);
            root->t = t;
            root->keys.push_back(std::make_unique<TK>(key));
        } else { // if the tree is not empty
            if (root->keys.size() == 2 * t - 1) { // if the root is full
                auto new_root = std::make_shared<Node>(false);
                new_root->t = t;
                new_root->children.push_back(root);
                new_root->split_child(0);
                root = new_root;
            }
            root->insert_non_full(key);
        }
    }

    bool search(TK key) {
        return search(root, key);
    }

    void print() {
        if (root != nullptr) {
            print(root, 0);
        }
    }

private:
    bool search(std::shared_ptr<Node> node, TK key) {
        size_t index = 0;
        while (index < node->keys.size() && key > *node->keys[index]) {
            ++index;
        }
        if (index < node->keys.size() && key == *node->keys[index]) {
            return true;
        }
        if (node->leaf) {
            return false;
        }
        return search(node->children[index], key);
    }

    void print(std::shared_ptr<Node> node, int level) {
        if (node == nullptr) {
            return;
        }
        std::cout << "[";
        for (int i = node->keys.size() - 1; i > 0; --i) {
            std::cout << *node->keys[i] << ",";
        }
        std::cout << *node->keys[0];
        std::cout << "]" << std::endl;

        for (size_t i = node->children.size(); i > 0; --i) {
            for (int j = 0; j <= level; ++j) {
                std::cout << "   ";
            }
            print(node->children[i - 1], level + 1);
        }
    }
};
