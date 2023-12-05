#pragma once
#include <iostream>
#include <memory>
#include <tuple>

/* Arboles B+ */
/*
- Son una extensión de los árboles B, por tanto, sue estructura es similar.
- Una diferencia importante es que todos los datos se encuentran en las hojas 
  y estos están enlazados entre sí (child[0] es el siguiente nodo hoja).
- Los nodos internos son marcadores para las hojas.
- Un valor en un nodo interno es el mayor valor del subárbol izquierdo.
*/

template <typename TK>
class BPlusTree {
private:
    struct Node {
        int min_degree;
        int num_keys;
        bool is_leaf;

        std::shared_ptr<Node> parent;
        std::unique_ptr<TK[]> keys;
        std::unique_ptr<std::shared_ptr<Node>[]> children; // for leaf nodes, children[0] is the next leaf node

        Node(bool is_leaf, int min_degree) : is_leaf(is_leaf), min_degree(min_degree), num_keys(0) {
            parent = nullptr;
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

        void range_search(TK start, TK end, std::tuple<TK>& result);


    }

    std::shared_ptr<Node> root;
    int min_degree;

public:
    BPlusTree(int min_degree) : min_degree(min_degree) {
        root = std::make_shared<Node>(true, min_degree);
    }

    bool search(TK key) {
        return (root != nullptr) ? root->search(key) : false;
    }

    std::tuple<TK> range_search(TK start, TK end) {
        std::tuple<TK> result;
        root->range_search(start, end, result);
        return result;
    }

    void insert(TK key) {
        if (root == nullptr) {
            root = std::make_shared<Node>(true, min_degree);
            root->keys[0] = key;
            root->num_keys = 1;
        } else {
            if (root->num_keys == 2 * min_degree - 1) {
                
            } else {
                
            }
        }
    }
};
