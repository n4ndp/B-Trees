#include <iostream>
#include <fstream>
#include "../include/Trees/b_tree.hpp"
#include "../include/Trees/b_plus_tree.hpp"
#include "../include/json.hpp"

// Overload operator<< for std::pair
template <typename T, typename U>
std::ostream& operator<<(std::ostream& os, const std::pair<T, U>& pair) {
    os << "(" << pair.first << ", " << pair.second << ")";
    return os;
}

int main(int argc, char const *argv[]) {
    std::string file_path = "../data/transactions.json";

    std::ifstream input_file(file_path);
    if (!input_file.is_open()) {
        std::cerr << "Could not open file: " << file_path << std::endl;
        return 1;
    }

    nlohmann::json json_data;
    try {
        input_file >> json_data;
    } catch (nlohmann::json::parse_error& e) {
        std::cerr << "JSON parse error: " << e.what() << std::endl;
        return 1;
    }

    BTree<double, std::pair<std::string, std::string>> b_tree(2);
    BPlusTree<double, std::pair<std::string, std::string>> b_plus_tree(2);

    for (const auto& person : json_data.items()) {
        for (const auto& transaction : person.value()) {
            double amount = transaction["Amount"];
            std::string id = transaction["TransactionID"];
            auto pair = std::make_pair(person.key(), id);
            b_tree.insert(amount, pair);
            b_plus_tree.insert(amount, pair);
        }
    }

    std::cout << "B Tree: " << std::endl;
    b_tree.pretty_print();
    std::cout << std::endl;

    std::cout << "Range search [100.0, 200.0]: " << std::endl;
    auto v = b_tree.range_search(100.0, 200.0);
    for (const auto& pair : v) {
        std::cout << pair << std::endl;
    }

    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "B+ Tree: " << std::endl;
    b_plus_tree.pretty_print();
    std::cout << std::endl;

    std::cout << "Range search [100.0, 200.0]: " << std::endl;
    v = b_plus_tree.range_search(100.0, 200.0);
    for (const auto& pair : v) {
        std::cout << pair << std::endl;
    }
    
    return 0;
}
