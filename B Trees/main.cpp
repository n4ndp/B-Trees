#include "b_tree.hpp"

int main() {
    BTree<int> tree(2);
    for (int i = 1; i <= 14; i++) {
        tree.insert(i);
    }
    tree.inorder();
    return 0;
}
