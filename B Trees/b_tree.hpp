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
            while (i < keys.size() && *key > *keys[i]) {
                i++;
            }

            if (is_leaf) { // CASE 1: node is a leaf
                keys.insert(keys.begin() + i, std::move(key));
                values.insert(values.begin() + i, std::move(value));
            } else { // CASE 2: node is not a leaf
                auto [median, new_child] = children[i]->insert(std::move(key), std::move(value));
                if (new_child) {
                    keys.insert(keys.begin() + i, std::move(median.first));
                    values.insert(values.begin() + i, std::move(median.second));
                    children.insert(children.begin() + i + 1, std::move(new_child));
                }
            }

            // if the node is full (has more than 2 * min_degree - 1 keys), split it
            if (keys.size() > 2 * min_degree - 1) {
                auto median_key = std::move(keys[min_degree - 1]);
                auto median_value = std::move(values[min_degree - 1]);
                auto right_split = std::make_unique<Node>(min_degree, is_leaf);

                for (int i = min_degree; i < keys.size(); i++) {
                    right_split->keys.push_back(std::move(keys[i]));
                    right_split->values.push_back(std::move(values[i]));
                }
                keys.erase(keys.begin() + min_degree - 1, keys.end());
                values.erase(values.begin() + min_degree - 1, values.end());

                if (!is_leaf) {
                    for (int i = min_degree; i < children.size(); i++) {
                        right_split->children.push_back(std::move(children[i]));
                    }
                    children.erase(children.begin() + min_degree, children.end());
                }

                return std::make_pair(std::make_pair(std::move(median_key), std::move(median_value)), std::move(right_split));
            }

            return std::make_pair(std::make_pair(nullptr, nullptr), nullptr);
        }
        V& search(K key) {
            // find the index
            int i = 0;
            while (i < keys.size() && key > *keys[i]) {
                i++;
            }

            if (i < keys.size() && key == *keys[i]) {
                return *values[i];
            } else if (is_leaf) {
                throw std::runtime_error("key not found");
            } else {
                return children[i]->search(key);
            }
        }
        void pretty_print(int depth = 0) {
            for (int i = 0; i < keys.size(); i++) {
                if (!is_leaf) {
                    children[i]->pretty_print(depth + 1);
                }
                for (int j = 0; j < depth; j++) {
                    std::cout << "  ";
                }
                std::cout << *keys[i] << std::endl;
            }
            if (!is_leaf) {
                children[keys.size()]->pretty_print(depth + 1);
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

        if (!root) {
            root = std::make_unique<Node>(min_degree, true);
            root->keys.push_back(std::move(k));
            root->values.push_back(std::move(v));
        } else {
            auto [median, new_child] = root->insert(std::move(k), std::move(v));
            if (new_child) {
                auto new_root = std::make_unique<Node>(min_degree, false);
                new_root->keys.push_back(std::move(median.first));
                new_root->values.push_back(std::move(median.second));
                new_root->children.push_back(std::move(root));
                new_root->children.push_back(std::move(new_child));
                root = std::move(new_root);
            }
        }
    }
    V& search(K key) {
        return root->search(key);
    }
    void pretty_print() {
        root->pretty_print();
    }
};
