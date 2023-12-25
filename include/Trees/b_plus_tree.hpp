#pragma once
#include <iostream>
#include <vector>
#include <memory>

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
        virtual V& search(K key) = 0;
        virtual void range_search(K lower_bound, K upper_bound, std::vector<std::pair<K, V>>& result) = 0;
        virtual void pretty_print(int depth = 0) = 0;
    };

    struct InternalNode : public Node {
        std::vector<k__ptr> keys;
        std::vector<std::shared_ptr<Node>> children;

        InternalNode(int min_degree) : Node(min_degree, false) {}

        std::pair<k__ptr, std::shared_ptr<Node>> insert(k__ptr key, v__ptr value) override {
            // find the index
            int i = 0;
            while (i < this->keys.size() && *key > *this->keys[i]) {
                i++;
            }
            
            auto [median, new_child] = this->children[i]->insert(std::move(key), std::move(value));

            if (new_child) {
                this->keys.insert(this->keys.begin() + i, median);
                this->children.insert(this->children.begin() + i + 1, new_child);

                // if the node is full (has more than 2 * min_degree - 1 keys), split it
                if (this->keys.size() > 2 * this->min_degree - 1) {
                    auto median_key = this->keys[this->min_degree - 1];
                    auto right_split = std::make_shared<InternalNode>(this->min_degree);
                    //right_split->parent = this->parent;

                    for (int i = this->min_degree; i < this->keys.size(); i++) {
                        right_split->keys.push_back(this->keys[i]);
                    }
                    this->keys.erase(this->keys.begin() + this->min_degree - 1, this->keys.end());

                    for (int i = this->min_degree; i < this->children.size(); i++) {
                        right_split->children.push_back(this->children[i]);
                    }
                    this->children.erase(this->children.begin() + this->min_degree, this->children.end());

                    return std::make_pair(median_key, right_split);
                }
            }

            return std::make_pair(nullptr, nullptr);
        }

        V& search(K key) override {
            // find the index
            int i = 0;
            while (i < this->keys.size() && key >= *this->keys[i]) {
                i++;
            }

            return this->children[i]->search(key);
        }

        void range_search(K lower_bound, K upper_bound, std::vector<std::pair<K, V>>& result) override {
            // find the index
            int i = 0;
            while (i < this->keys.size() && lower_bound >= *this->keys[i]) {
                i++;
            }

            this->children[i]->range_search(lower_bound, upper_bound, result);
        }

        void pretty_print(int depth = 0) override {
            for (int i = 0; i < this->keys.size(); i++) {
                this->children[i]->pretty_print(depth + 1);
                for (int j = 0; j < depth; j++) {
                    std::cout << "  ";
                }
                std::cout << *this->keys[i] << std::endl;
            }
            this->children[this->keys.size()]->pretty_print(depth + 1);
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
            while (i < this->keys.size() && *key > *this->keys[i]) {
                i++;
            }

            this->keys.insert(this->keys.begin() + i, std::move(key));
            this->values.insert(this->values.begin() + i, std::move(value));

            // if the node is full (has more than 2 * min_degree - 1 keys), split it
            if (this->keys.size() > 2 * this->min_degree - 1) {
                auto right_split = std::make_shared<LeafNode>(this->min_degree);
                //right_split->parent = this->parent;

                for (int i = this->min_degree; i < this->keys.size(); i++) {
                    right_split->keys.push_back(std::move(this->keys[i]));
                    right_split->values.push_back(std::move(this->values[i]));
                }

                this->keys.erase(this->keys.begin() + this->min_degree, this->keys.end());
                this->values.erase(this->values.begin() + this->min_degree, this->values.end());

                right_split->next = this->next;
                this->next = right_split;

                return std::make_pair(right_split->keys[0], right_split);
            }

            return std::make_pair(nullptr, nullptr);
        }

        V& search(K key) override {
            for (int i = 0; i < this->keys.size(); i++) {
                if (*this->keys[i] == key) {
                    return *this->values[i];
                }
            }

            throw std::runtime_error("Key not found");
        }

        void range_search(K lower_bound, K upper_bound, std::vector<std::pair<K, V>>& result) override {
            for (int i = 0; i < this->keys.size(); i++) {
                if (*this->keys[i] >= lower_bound && *this->keys[i] <= upper_bound) {
                    result.push_back(std::make_pair(*this->keys[i], *this->values[i]));
                }
            }

            if (this->next != nullptr) {
                this->next->range_search(lower_bound, upper_bound, result);
            }
        }

        void pretty_print(int depth = 0) override {
            for (int i = 0; i < this->keys.size(); i++) {
                for (int j = 0; j < depth; j++) {
                    std::cout << "   ";
                }
                std::cout << *this->keys[i] << ": " << *this->values[i] << std::endl;
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

        if (this->root == nullptr) {
            auto new_root = std::make_shared<LeafNode>(this->min_degree);
            new_root->keys.push_back(std::move(k));
            new_root->values.push_back(std::move(v));

            this->root = new_root;
        } else {
            auto [median, new_child] = this->root->insert(std::move(k), std::move(v));

            if (new_child) {
                auto new_root = std::make_shared<InternalNode>(this->min_degree);
                new_root->keys.push_back(median);
                new_root->children.push_back(this->root);
                new_root->children.push_back(new_child);

                //this->root->parent = new_root;
                //new_child->parent = new_root;

                this->root = new_root;
            }
        }
    }

    V& search(K key) {
        return this->root->search(key);
    }

    std::vector<std::pair<K, V>> range_search(K lower_bound, K upper_bound) {
        std::vector<std::pair<K, V>> result;
        this->root->range_search(lower_bound, upper_bound, result);
        return result;
    }

    void pretty_print() {
        this->root->pretty_print();
    }
};
