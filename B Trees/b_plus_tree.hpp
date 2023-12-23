#pragma once
#include <iostream>
#include <vector>
#include <memory>

// B+ Tree example:
// [13]
// |____[2,4]
// |    |____[1]
// |    |____[2,3]
// |    |____[4,11]
// |____[14]
//      |____[13]
//      |____[14,15]

template <
    typename K, // key type
    typename V> // value type
class BPlusTree {
    using k__ptr = std::shared_ptr<K>;
    using v__ptr = std::unique_ptr<V>;

    struct Node {
        int min_degree;
        bool is_leaf;
        std::shared_ptr<Node> parent;

        Node(int min_degree, bool is_leaf) : min_degree(min_degree), is_leaf(is_leaf), parent(nullptr) {}

        virtual std::pair<k__ptr, std::shared_ptr<Node>> insert(k__ptr key, v__ptr value) = 0;
        virtual void pretty_print(int depth = 0) = 0;
    };

    struct InternalNode : public Node {
        std::vector<k__ptr> keys;
        std::vector<std::shared_ptr<Node>> children;

        InternalNode(int min_degree) : Node(min_degree, false) {}

        std::pair<k__ptr, std::shared_ptr<Node>> insert(k__ptr key, v__ptr value) override {
            // find the index
            int i = 0;
            while (i < keys.size() && *key > *keys[i]) {
                i++;
            }
            
            auto [median, new_child] = children[i]->insert(std::move(key), std::move(value));

            if (new_child) {
                keys.insert(keys.begin() + i, median);
                children.insert(children.begin() + i + 1, new_child);

                // if the node is full (has more than 2 * min_degree - 1 keys), split it
                if (keys.size() > 2 * this->min_degree - 1) {
                    auto median_key = keys[this->min_degree - 1];
                    std::shared_ptr<Node> parent = this->parent; // Declare and assign 'parent'
                    auto right_split = std::make_shared<InternalNode>(this->min_degree);
                    right_split->parent = parent;

                    for (int i = this->min_degree; i < keys.size(); i++) {
                        right_split->keys.push_back(keys[i]);
                    }
                    keys.erase(keys.begin() + this->min_degree - 1, keys.end());

                    for (int i = this->min_degree; i < children.size(); i++) {
                        right_split->children.push_back(children[i]);
                    }
                    children.erase(children.begin() + this->min_degree, children.end());

                    return std::make_pair(median_key, right_split);
                }
            }

            return std::make_pair(nullptr, nullptr);
        }
        void pretty_print(int depth = 0) override {
            for (int i = 0; i < keys.size(); i++) {
                children[i]->pretty_print(depth + 1);
                for (int j = 0; j < depth; j++) {
                    std::cout << "  ";
                }
                std::cout << *keys[i] << std::endl;
            }
            children[keys.size()]->pretty_print(depth + 1);
        }
    };

    struct LeafNode : public Node {
        std::vector<k__ptr> keys;
        std::vector<v__ptr> values;
        std::shared_ptr<LeafNode> next;

        LeafNode(int min_degree) : Node(min_degree, true) {}

        std::pair<k__ptr, std::shared_ptr<Node>> insert(k__ptr key, v__ptr value) override {
            // find the index
            int i = 0;
            while (i < keys.size() && *key > *keys[i]) {
                i++;
            }

            keys.insert(keys.begin() + i, std::move(key));
            values.insert(values.begin() + i, std::move(value));

            // if the node is full (has more than 2 * min_degree - 1 keys), split it
            if (keys.size() > 2 * this->min_degree - 1) {
                std::shared_ptr<Node> parent = this->parent; // Declare and assign 'parent'
                auto right_split = std::make_shared<LeafNode>(this->min_degree);
                right_split->parent = parent;

                for (int i = this->min_degree; i < keys.size(); i++) {
                    right_split->keys.push_back(std::move(keys[i]));
                    right_split->values.push_back(std::move(values[i]));
                }

                keys.erase(keys.begin() + this->min_degree, keys.end());
                values.erase(values.begin() + this->min_degree, values.end());

                right_split->next = next;
                next = right_split;

                return std::make_pair(right_split->keys[0], right_split);
            }

            return std::make_pair(nullptr, nullptr);
        }
        void pretty_print(int depth = 0) override {
            for (int i = 0; i < keys.size(); i++) {
                for (int j = 0; j < depth; j++) {
                    std::cout << "  ";
                }
                std::cout << *keys[i] << std::endl;
            }
        }
    };

private:
    std::shared_ptr<Node> root;
    int min_degree;

public:
    BPlusTree(int min_degree) : min_degree(min_degree), root(nullptr) {}

    void insert(K key, V value) {
        auto k = std::make_shared<K>(key);
        auto v = std::make_unique<V>(value);

        if (root == nullptr) {
            auto new_root = std::make_shared<LeafNode>(min_degree);
            new_root->keys.push_back(std::move(k));
            new_root->values.push_back(std::move(v));

            root = new_root;
        } else {
            auto [median, new_child] = root->insert(std::move(k), std::move(v));

            if (new_child) {
                auto new_root = std::make_shared<InternalNode>(min_degree);
                new_root->keys.push_back(median);
                new_root->children.push_back(root);
                new_root->children.push_back(new_child);

                root->parent = new_root;
                new_child->parent = new_root;

                root = new_root;
            }
        }
    }
    void pretty_print() {
        root->pretty_print();
    }
};
