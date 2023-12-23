#pragma once
#include <iostream>
#include <vector>
#include <memory>

template <
    typename K, // key type
    typename V> // value type
class BTree {
    using k__ptr = std::unique_ptr<K>;
    using v__ptr = std::unique_ptr<V>;

    struct Node {
        int min_degree;
        bool is_leaf;
        std::vector<k__ptr> keys;
        std::vector<v__ptr> values;
        std::vector<std::unique_ptr<Node>> children;

        Node(int min_degree, bool is_leaf) : min_degree(min_degree), is_leaf(is_leaf) {}

        std::pair<std::pair<k__ptr, v__ptr>, std::unique_ptr<Node>> insert(k__ptr key, v__ptr value) {
            // find the index
            int i = 0;
            while (i < this->keys.size() && *key > *this->keys[i]) {
                i++;
            }

            if (this->is_leaf) { // CASE 1: node is a leaf
                this->keys.insert(this->keys.begin() + i, std::move(key));
                this->values.insert(this->values.begin() + i, std::move(value));
            } else { // CASE 2: node is not a leaf
                auto [median, new_child] = this->children[i]->insert(std::move(key), std::move(value));
                if (new_child) {
                    this->keys.insert(this->keys.begin() + i, std::move(median.first));
                    this->values.insert(this->values.begin() + i, std::move(median.second));
                    this->children.insert(this->children.begin() + i + 1, std::move(new_child));
                }
            }

            // if the node is full (has more than 2 * min_degree - 1 keys), split it
            if (this->keys.size() > 2 * this->min_degree - 1) {
                auto median_key = std::move(this->keys[this->min_degree - 1]);
                auto median_value = std::move(this->values[this->min_degree - 1]);
                auto right_split = std::make_unique<Node>(this->min_degree, this->is_leaf);

                for (int i = this->min_degree; i < this->keys.size(); i++) {
                    right_split->keys.push_back(std::move(this->keys[i]));
                    right_split->values.push_back(std::move(this->values[i]));
                }
                this->keys.erase(this->keys.begin() + this->min_degree - 1, this->keys.end());
                this->values.erase(this->values.begin() + this->min_degree - 1, this->values.end());

                if (!this->is_leaf) {
                    for (int i = this->min_degree; i < this->children.size(); i++) {
                        right_split->children.push_back(std::move(this->children[i]));
                    }
                    this->children.erase(this->children.begin() + this->min_degree, this->children.end());
                }

                return std::make_pair(std::make_pair(std::move(median_key), std::move(median_value)), std::move(right_split));
            }

            return std::make_pair(std::make_pair(nullptr, nullptr), nullptr);
        }

        V& search(K key) {
            // find the index
            int i = 0;
            while (i < this->keys.size() && key > *this->keys[i]) {
                i++;
            }

            if (i < this->keys.size() && key == *this->keys[i]) {
                return *this->values[i];
            } else if (this->is_leaf) {
                throw std::runtime_error("key not found");
            } else {
                return this->children[i]->search(key);
            }
        }

        void pretty_print(int depth = 0) {
            for (int i = 0; i < this->keys.size(); i++) {
                if (!this->is_leaf) {
                    this->children[i]->pretty_print(depth + 1);
                }
                for (int j = 0; j < depth; j++) {
                    std::cout << "   ";
                }
                std::cout << *this->keys[i] << ": " << *this->values[i] << std::endl;
            }
            if (!this->is_leaf) {
                this->children[this->keys.size()]->pretty_print(depth + 1);
            }
        }
    };

private:
    std::unique_ptr<Node> root;
    int min_degree;

public:
    BTree(int min_degree) : min_degree(min_degree), root(nullptr) {}

    void insert(K key, V value) {
        auto k = std::make_unique<K>(key);
        auto v = std::make_unique<V>(value);

        if (this->root == nullptr) {
            auto new_root = std::make_unique<Node>(this->min_degree, true);
            new_root->keys.push_back(std::move(k));
            new_root->values.push_back(std::move(v));

            this->root = std::move(new_root);
        } else {
            auto [median, new_child] = this->root->insert(std::move(k), std::move(v));

            if (new_child) {
                auto new_root = std::make_unique<Node>(this->min_degree, false);
                new_root->keys.push_back(std::move(median.first));
                new_root->values.push_back(std::move(median.second));
                new_root->children.push_back(std::move(this->root));
                new_root->children.push_back(std::move(new_child));
                this->root = std::move(new_root);
            }
        }
    }

    V& search(K key) {
        return this->root->search(key);
    }

    void pretty_print() {
        this->root->pretty_print();
    }
};
