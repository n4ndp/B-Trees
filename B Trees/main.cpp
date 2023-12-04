#include "b_tree.hpp"
#include <iostream>

int main() {
    BTree<int> tree(2);
    /*
        minimum degree = 3
        maximun degree = 2 * 2 = 4
    */
    for (int i = 1; i <= 22; ++i) {
        tree.insert(i);
    }

    std::cout << "tree.search(10) = " << tree.search(10) << std::endl;
    tree.print();

    return 0;
}
