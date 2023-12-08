#pragma once
#include <iostream>
#include <memory>

template <typename TK, typename TV>
class BTree {
private:
    struct Node {
        int min_degree; // Minimum degree (defines the range for number of keys)
        int num_keys; // Current number of keys
        bool is_leaf; // Is true when node is leaf. Otherwise false

        // pair<key, value>
        std::unique_ptr<std::pair<TK, TV>[]> keys;
        std::unique_ptr<std::shared_ptr<Node>[]> children;

        Node(bool is_leaf, int min_degree) : is_leaf(is_leaf), min_degree(min_degree), num_keys(0) {
            keys = std::make_unique<std::pair<TK, TV>[]>(2 * min_degree - 1); // Allocate memory for maximum number of possible keys
            children = std::make_unique<std::shared_ptr<Node>[]>(2 * min_degree); // Allocate memory for maximum number of possible children
        }
    };

    std::shared_ptr<Node> root;
    int min_degree;

public: // public functions
    BTree(int min_degree) : min_degree(min_degree) {
        root = nullptr;
    }

    bool search(TK key) {
        return search(root, key) != nullptr;
    }

    void insert(TK key, TV value) {
        if (root == nullptr) { // Tree is empty
            root = std::make_shared<Node>(true, min_degree);
            root->keys[0] = std::make_pair(key, value);
            root->num_keys = 1;
        } else { // Tree is not empty
            if (root->num_keys == 2 * min_degree - 1) { // Root is full
                std::shared_ptr<Node> new_root = std::make_shared<Node>(false, min_degree);
                new_root->children[0] = root;
                split_child(new_root, 0, root);
                int index = 0;
                if (new_root->keys[0].first < key) {
                    ++index;
                }
                insert_non_full(new_root->children[index], key, value);
                root = new_root;
            } else { // Root is not full
                insert_non_full(root, key, value);
            }
        }
    }

    void pretty_print() {
        pretty_print(root, 0);
    }

private: // helper functions
    int find_key(std::shared_ptr<Node> node, TK key) {
        int index = 0;
        while (index < node->num_keys && node->keys[index].first < key) { // Find the first key greater than or equal to k
            ++index;
        }
        return index;
    }

    std::shared_ptr<Node> search(std::shared_ptr<Node> node, TK key) {
        int index = find_key(node, key);

        if (index < node->num_keys && node->keys[index].first == key) { // Found key
            return node;
        }

        if (node->is_leaf) {
            return nullptr;
        }

        return search(node->children[index], key); // Recursively search the children
    }
    
    void insert_non_full(std::shared_ptr<Node> node, TK key, TV value) {
        int index = node->num_keys - 1;

        if (node->is_leaf) { // Node is leaf
            while (index >= 0 && node->keys[index].first > key) {
                node->keys[index + 1] = node->keys[index];
                --index;
            }

            node->keys[index + 1] = std::make_pair(key, value);
            ++node->num_keys;
        } else { // Node is not leaf (internal node)
            while (index >= 0 && node->keys[index].first > key) {
                --index;
            }
            index += 1;

            if (node->children[index]->num_keys == 2 * min_degree - 1) {
                split_child(node, index, node->children[index]);

                if (node->keys[index].first < key) {
                    ++index;
                }
            }
            insert_non_full(node->children[index], key, value);
        }
    }

    void split_child(std::shared_ptr<Node> node, int index, std::shared_ptr<Node> child) {
        std::shared_ptr<Node> new_child = std::make_shared<Node>(child->is_leaf, min_degree);
        new_child->num_keys = min_degree - 1;

        for (int i = 0; i < min_degree - 1; ++i) { // Copy the last min_degree - 1 keys of child to new_child
            new_child->keys[i] = child->keys[i + min_degree];
        }

        if (!child->is_leaf) { // Copy the last min_degree children of child to new_child
            for (int i = 0; i < min_degree; ++i) {
                new_child->children[i] = child->children[i + min_degree];
            }
        }

        child->num_keys = min_degree - 1;

        for (int i = node->num_keys; i >= index + 1; --i) { // Make room for new child
            node->children[i + 1] = node->children[i];
        }
        node->children[index + 1] = new_child;

        for (int i = node->num_keys - 1; i >= index; --i) {
            node->keys[i + 1] = node->keys[i];
        }
        node->keys[index] = child->keys[min_degree - 1]; // Move the middle key of child to node

        ++node->num_keys;
    }

    void pretty_print(std::shared_ptr<Node> node, int depth) {
        if (node != nullptr) {
            for (int i = node->num_keys - 1; i >= 0; --i) {
                if (!node->is_leaf) {
                    pretty_print(node->children[i + 1], depth + 1);
                }
                std::cout << std::string(depth * 4, ' ') << node->keys[i].first << ":" << node->keys[i].second << std::endl;
            }
            if (!node->is_leaf) {
                pretty_print(node->children[0], depth + 1);
            }
        }
    }
};
