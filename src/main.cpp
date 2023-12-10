#include "AST.hpp"

#include <iostream>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        return 1;
    }

    try {
        tree::AST ast{static_cast<std::string>(argv[1])};
        std::cout << "Equation: " << ast.getEquation();
        std::cout << " Result: " << ast.getResult() << std::endl;
    } catch (const char* err) {
        std::cout << err << std::endl;
    }

    return 0;
}
