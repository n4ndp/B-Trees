#include "b_tree.hpp"

void btree() {
    BTree<int, std::string> tree(2);

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
    tree.insert(11, "eleven");

    std::cout << std::endl;

    tree.pretty_print();
    tree.search(1) = "ONE";
    tree.search(2) = "TWO";
    tree.search(3) = "THREE";

    std::cout << std::endl;

    for (int i = 1; i <= 11; i++) {
        std::cout << "tree.search(" << i << ") = " << tree.search(i) << std::endl;
    }
}

int main() {
    btree();

    return 0;
}
