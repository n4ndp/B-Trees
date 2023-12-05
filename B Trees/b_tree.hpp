#include <iostream>
#include <memory>

template <typename TK>
class BTree {
private:
    struct Node {
        int min_degree;
        int num_keys;
        bool is_leaf;

        std::unique_ptr<TK[]> keys;
        std::unique_ptr<std::shared_ptr<Node>[]> children;

        Node(bool is_leaf, int min_degree) : is_leaf(is_leaf), min_degree(min_degree), num_keys(0) {
            keys = std::make_unique<TK[]>(2 * min_degree - 1);
            children = std::make_unique<std::shared_ptr<Node>[]>(2 * min_degree);
        }

        bool search(TK key) {
            int i = 0;
            while (i < num_keys && key > keys[i]) { // find the first key that is greater than or equal to the key
                i++;
            }
            if (keys[i] == key) {
                return true; // found
            }
            if (is_leaf) {
                return false; // not found
            }
            return children[i]->search(key); // recursively search the subtree
        }

        void insert_non_full(TK key) {
            int i = num_keys - 1;
            if (is_leaf) { // leaf node
                while (i >= 0 && key < keys[i]) { // find the first key that is less than or equal to the key
                    keys[i + 1] = keys[i];
                    i--;
                }
                keys[i + 1] = key;
                num_keys++;
            } else { // internal node
                while (i >= 0 && key < keys[i]) { // find the first key that is less than or equal to the key
                    i--;
                }
                i++;
                if (children[i]->num_keys == 2 * min_degree - 1) { // child is full
                    split_child(i); // split the child
                    if (key > keys[i]) {
                        i++;
                    }
                }
                children[i]->insert_non_full(key); // recursively insert the key
            }
        }

        void split_child(int i) {
            auto child = children[i];
            auto new_child = std::make_shared<Node>(child->is_leaf, min_degree);
            new_child->num_keys = min_degree - 1;

            for (int j = 0; j < min_degree - 1; j++) { // copy the last min_degree - 1 keys to the new child
                new_child->keys[j] = child->keys[j + min_degree];
            }
            if (!child->is_leaf) { // copy the last min_degree children to the new child if it is not a leaf node
                for (int j = 0; j < min_degree; j++) {
                    new_child->children[j] = child->children[j + min_degree];
                }
            }

            child->num_keys = min_degree - 1;
            for (int j = num_keys; j >= i + 1; j--) { // make room for the new child
                children[j + 1] = children[j];
            }

            children[i + 1] = new_child;
            for (int j = num_keys - 1; j >= i; j--) { // make room for the new key
                keys[j + 1] = keys[j];
            }
            keys[i] = child->keys[min_degree - 1];
            num_keys++;
        }
    };

    std::shared_ptr<Node> root;
    int min_degree;

public:
    BTree(int min_degree) : min_degree(min_degree), root(nullptr) {}

    bool search(TK key) {
        return (root != nullptr) ? root->search(key) : false;
    }

    void insert(TK key) {
        if (root == nullptr) {
            root = std::make_shared<Node>(true, min_degree);
            root->keys[0] = key;
            root->num_keys++;
        } else {
            if (root->num_keys == 2 * min_degree - 1) {
                auto new_root = std::make_shared<Node>(false, min_degree);
                new_root->children[0] = root;
                new_root->split_child(0);

                int i = 0;
                if (new_root->keys[0] < key) {
                    i++;
                }

                new_root->children[i]->insert_non_full(key);
                root = new_root;
            } else {
                root->insert_non_full(key);
            }
        }
    }

    void inorder() const {
        if (root != nullptr) {
            inorder(root);
        }
    }

private:
    void inorder(std::shared_ptr<Node> node) const {
        if (node->is_leaf) {
            for (int i = 0; i < node->num_keys; i++) {
                std::cout << node->keys[i] << " ";
            }
        } else {
            for (int i = 0; i < node->num_keys; i++) {
                inorder(node->children[i]);
                std::cout << node->keys[i] << " ";
            }
            inorder(node->children[node->num_keys]);
        }
    }
};
