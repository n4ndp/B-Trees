#include "b_tree.hpp"

int main() {
    BTree<int> tree(2);
    tree.insert(4);
    tree.insert(2);
    tree.insert(20);
    tree.insert(10);
    tree.insert(1);
    tree.insert(14);
    tree.insert(7);
    tree.insert(11);
    tree.insert(3);
    tree.insert(9);
    tree.insert(8);
    
    tree.inorder();

    return 0;
}
