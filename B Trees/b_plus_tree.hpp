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
        
        virtual std::pair<k__ptr, std::unique_ptr<Node>> insert(k__ptr key, v__ptr value) = 0;
    };

    struct InternalNode : public Node {
        std::vector<k__ptr> keys;
        std::vector<std::shared_ptr<Node>> children;

        InternalNode(int min_degree) : Node(min_degree, false) {}

        std::pair<k__ptr, std::unique_ptr<Node>> insert(k__ptr key, v__ptr value) override {
            
        }
    };

    struct LeafNode : public Node {
        std::vector<k__ptr> keys;
        std::vector<v__ptr> values;
        std::unique_ptr<LeafNode> next;

        LeafNode(int min_degree) : Node(min_degree, true) {}

        std::pair<k__ptr, std::unique_ptr<Node>> insert(k__ptr key, v__ptr value) override {
            // find the index
            int i = 0;
            while (i < keys.size() && *key > *keys[i]) {
                i++;
            }

            keys.insert(keys.begin() + i, std::move(key));
            values.insert(values.begin() + i, std::move(value));

            // if the node is full (has more than 2 * min_degree - 1 keys), split it
            if (keys.size() > 2 * min_degree - 1) {
                auto right_split = std::make_unique<LeafNode>(min_degree);

                for (int i = min_degree; i < keys.size(); i++) {
                    right_split->keys.push_back(std::move(keys[i]));
                    right_split->values.push_back(std::move(values[i]));
                }
                keys.erase(keys.begin() + min_degree, keys.end());
                values.erase(values.begin() + min_degree, values.end());

                // update next pointer
                right_split->next = std::move(next);
                next = std::move(right_split);

                
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
            root = std::make_shared<LeafNode>(min_degree);
            root->keys.push_back(std::move(k));
            root->values.push_back(std::move(v));
        } else {
            auto [median, new_child] = insert(root, k, std::move(v));

            if (new_child) {
                
            }
        }
    }
};


// #pragma once
// #include <iostream>
// #include <memory>
// #include <tuple>

// /* Arboles B+ */ una llave en un nodo interno es el maximo valor llave de su subarbol izquierdo en un b+?
// /*
// - Son una extensión de los árboles B, por tanto, sue estructura es similar.
// - Una diferencia importante es que todos los datos se encuentran en las hojas 
//   y estos están enlazados entre sí (child[0] es el siguiente nodo hoja).
// - Los nodos internos son marcadores para las hojas.
// - Un valor en un nodo interno es el mayor valor del subárbol izquierdo.
// */

// template <typename TK, typename TV>
// class BPlusTree {
// private:
//     struct Node {
//         int min_degree;
//         int num_keys;
//         bool is_leaf;

//         std::shared_ptr<Node> parent;

//     protected:
//         Node(int min_degree, bool is_leaf) :
//              min_degree(min_degree), num_keys(0), is_leaf(is_leaf), parent(nullptr) {}
//     };

//     struct InternalNode : public Node {
//         std::unique_ptr<TK[]> keys;
//         std::unique_ptr<std::shared_ptr<Node>[]> children;

//         InternalNode(int min_degree) : Node(min_degree, false) {
//             keys = std::make_unique<TK[]>(2 * min_degree - 1);
//             children = std::make_unique<std::shared_ptr<Node>[]>(2 * min_degree);
//         }
//     };

//     struct LeafNode : public Node {
//         std::unique_ptr<std::pair<TK, TV>[]> values;
//         std::unique_ptr<LeafNode> next;

//         LeafNode(int min_degree) : Node(min_degree, true) {
//             values = std::make_unique<std::pair<TK, TV>[]>(2 * min_degree - 1);
//             next = nullptr;
//         }
//     };

//     std::shared_ptr<Node> root;
//     int min_degree;

// public: // public functions

//     BPlusTree(int min_degree) : min_degree(min_degree) {
//         root = nullptr;
//     }

//     // bool search(TK key) {
//     //     return search(root, key) != nullptr;
//     // }

//     // void insert(TK key, TV value) {
//     //     if (root == nullptr) { // Tree is empty
//     //         root = std::make_shared<Node>(true, min_degree, nullptr);
//     //         root->keys[0] = std::make_pair(key, value);
//     //         root->num_keys = 1;
//     //     } else { // Tree is not empty
//     //         std::shared_ptr<Node> node_to_insert = search(root, key);

//     //         if (node_to_insert->num_keys < 2 * min_degree - 1) { // Leaf node is not full
//     //             insert_non_full(node_to_insert, key, value);

//     //             // Update the next pointer of the leaf node
//     //             node_to_insert->children[node_to_insert->num_keys] = node_to_insert->children[node_to_insert->num_keys - 1];
//     //             node_to_insert->children[node_to_insert->num_keys - 1] = nullptr;
//     //         } else { // Leaf node is full
//     //             std::shared_ptr<Node> new_node = std::make_shared<Node>(true, min_degree, node_to_insert->parent);

                

//     //             /*// Update next pointer of the leaf new node
//     //             new_node->children[min_degree - 1] = node_to_insert->children[node_to_insert->num_keys];

//     //             // Copy keys and values to new node
//     //             for (int i = 0; i < min_degree - 1; ++i) {
//     //                 new_node->keys[i] = node_to_insert->keys[i + min_degree];
//     //                 new_node->num_keys++;
//     //                 node_to_insert->num_keys--;
//     //             }

//     //             // Update next pointer of the leaf node
//     //             node_to_insert->children[node_to_insert->num_keys] = new_node;

//     //             // Update parent pointers of the new node's children*/
                
//     //         }
//     //     }
//     // }

// private: // helper functions
//     // int find_key(std::shared_ptr<Node> node, TK key) {
//     //     int index = 0;
//     //     while (index < node->num_keys && node->keys[index].first < key) { // Find the first key greater than or equal to k
//     //         ++index;
//     //     }
//     //     return index;
//     // }

//     // std::shared_ptr<Node> search(std::shared_ptr<Node> node, TK key) {
//     //     int index = find_key(node, key);

//     //     if (index < node->num_keys && node->keys[index].first == key) {
//     //         if (node->is_leaf) { // Node is leaf and contains key
//     //             return node;
//     //         } else {
//     //             return search(node->children[index + 1], key);
//     //         }
//     //     } else if (node->is_leaf) { // Node is leaf and does not contain key
//     //         return nullptr;
//     //     } else {
//     //         return search(node->children[index], key);
//     //     }
//     // }

//     // void insert_non_full(std::shared_ptr<Node> node, TK key, TV value) {
//     //     int index = node->num_keys - 1;

//     //     while (index >= 0 && node->keys[index].first > key) {
//     //         node->keys[index + 1] = node->keys[index];
//     //         --index;
//     //     }

//     //     node->keys[index + 1] = std::make_pair(key, value);
//     //     ++node->num_keys;
//     // }
// };
