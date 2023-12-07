#include "btree.hpp"

int main() {
    BTree<int, std::string> tree(3);

    tree.insert(1, "one");
    tree.insert(2, "two");
    tree.insert(3, "three");
    tree.insert(4, "four");
    tree.insert(5, "five");
    tree.insert(6, "six");

    tree.print();

    return 0;
}
