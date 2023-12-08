#include "b_tree.hpp"

int main() {
    BTree<int, std::string> tree(3);

    tree.insert(1, "one");
    tree.insert(2, "two");
    tree.insert(3, "three");
    tree.insert(4, "four");
    tree.insert(5, "five");
    tree.insert(6, "six");
    tree.insert(7, "seven");
    tree.insert(8, "eight");
    tree.insert(9, "nine");
    tree.insert(10, "ten");

    tree.pretty_print();

    return 0;
}
