#pragma once
#include <iostream>
#include <vector>
#include <memory>

template <
	typename K, // key type parameter
	typename V> // value type parameter
class BTree {
    struct Node {
        int min_degree;
 		bool is_leaf;
        std::vector<std::unique_ptr<K>> keys;
        std::vector<std::unique_ptr<Node>> children;

        Node(int min_degree, bool is_leaf) : min_degree(min_degree), is_leaf(is_leaf) {}

        std::pair<std::unique_ptr<K>, std::unique_ptr<Node>> insert(std::unique_ptr<K> key, std::unique_ptr<V> value) {
            if (is_leaf) {
                // find the index to insert the key
                int index = 0;
                while (index < keys.size() && *key > *keys[index]) {
                    index++;
                }
                keys.insert(keys.begin() + index, std::move(key));

                // if the node has more than 2 * min_degree - 1 keys, split it
                if (keys.size() > 2 * min_degree - 1) {
                    std::unique_ptr<K> median_key = std::move(keys[min_degree - 1]);
                    std::unique_ptr<Node> right_split = std::make_unique<Node>(min_degree, true);

                    // move the right half of the keys to the right split
                    for (int i = min_degree; i < keys.size(); i++) {
                        right_split->keys.push_back(std::move(keys[i]));
                    }

                    // remove the right half of the keys from the current node
                    keys.erase(keys.begin() + min_degree - 1, keys.end());

                    return std::make_pair(std::move(median_key), std::move(right_split));
                }
            } else {
                // find the index of the child to insert the key
                int index = 0;
                while (index < keys.size() && *key > *keys[index]) {
                    index++;
                }

                // insert the key into the child
                std::pair<std::unique_ptr<K>, std::unique_ptr<Node>> result = children[index]->insert(std::move(key), std::move(value));
                if (result.first != nullptr) {
                    // if the child was split, insert the median key into the current node
                    keys.insert(keys.begin() + index, std::move(result.first));
                    children.insert(children.begin() + index + 1, std::move(result.second));

                    // if the node has more than 2 * min_degree - 1 keys, split it
                    if (keys.size() > 2 * min_degree - 1) {
                        std::unique_ptr<K> median_key = std::move(keys[min_degree - 1]);
                        std::unique_ptr<Node> right_split = std::make_unique<Node>(min_degree, false);

                        // move the right half of the keys to the right split
                        for (int i = min_degree; i < keys.size(); i++) {
                            right_split->keys.push_back(std::move(keys[i]));
                        }

                        // move the right half of the children to the right split
                        for (int i = min_degree; i < children.size(); i++) {
                            right_split->children.push_back(std::move(children[i]));
                        }

                        // remove the right half of the keys from the current node
                        keys.erase(keys.begin() + min_degree - 1, keys.end());

                        // remove the right half of the children from the current node
                        children.erase(children.begin() + min_degree, children.end());

                        return std::make_pair(std::move(median_key), std::move(right_split));
                    }
                }
            }
            return std::make_pair(nullptr, nullptr);
        }

        void pretty_print(int depth) {
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
    BTree(int min_degree) : min_degree(min_degree) {
		root = std::make_unique<Node>(min_degree, true);
	}

    void insert(K key, V value) {
        std::unique_ptr<K> key_ptr = std::make_unique<K>(key);
        std::unique_ptr<V> value_ptr = std::make_unique<V>(value);

        std::pair<std::unique_ptr<K>, std::unique_ptr<Node>> result = root->insert(std::move(key_ptr), std::move(value_ptr));
        if (result.first != nullptr) {
            // if the root was split, create a new root
            std::unique_ptr<Node> new_root = std::make_unique<Node>(min_degree, false);
            new_root->keys.push_back(std::move(result.first));
            new_root->children.push_back(std::move(root));
            new_root->children.push_back(std::move(result.second));
            root = std::move(new_root);
        }
    }

    void pretty_print() {
		root->pretty_print(0);
	}
};
